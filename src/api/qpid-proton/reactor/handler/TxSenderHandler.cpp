/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TxSenderHandler.cpp
 * Author: pematous
 *
 * Created on November 20, 2024
 */

#include "TxSenderHandler.h"

#include "reactor/formatter/ReactorDecoder.h"
#include "formatter/DictFormatter.h"
#include "formatter/DictWriter.h"

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;
using namespace dtests::common::log;

TxSenderHandler::TxSenderHandler(
    const string &url,
    vector<string> conn_url,
    bool is_topic,
    string user,
    string password,
    string sasl_mechanisms,
    string conn_sasl_enabled,
    string conn_ssl_certificate,
    string conn_ssl_private_key,
    string conn_ssl_password,
    string conn_ssl_trust_store,
    bool conn_ssl_verify_peer,
    bool conn_ssl_verify_peer_name,
    int timeout,
    int duration_time,
    string duration_mode,
    string conn_reconnect,
    int32_t conn_reconnect_interval,
    int32_t conn_reconnect_limit,
    int32_t conn_reconnect_timeout,
    uint32_t conn_reconnect_first,
    uint32_t conn_reconnect_increment,
    bool conn_reconnect_doubling,
    bool conn_reconnect_custom,
    uint32_t conn_heartbeat,
    uint32_t max_frame_size,
    bool conn_use_config_file,
    string log_msgs,
    string tx_action,
    string tx_endloop_action
)
    : super(
        url,
        conn_url,
        is_topic,
        user,
        password,
        sasl_mechanisms,
        conn_sasl_enabled,
        conn_ssl_certificate,
        conn_ssl_private_key,
        conn_ssl_password,
        conn_ssl_trust_store,
        conn_ssl_verify_peer,
        conn_ssl_verify_peer_name,
        timeout,
        duration_time,
        duration_mode,
        conn_reconnect,
        conn_reconnect_interval,
        conn_reconnect_limit,
        conn_reconnect_timeout,
        conn_reconnect_first,
        conn_reconnect_increment,
        conn_reconnect_doubling,
        conn_reconnect_custom,
        conn_heartbeat,
        max_frame_size,
        conn_use_config_file,
        log_msgs
    ),
    batch_size(0),
    current_batch(0),
    processed(0),
    tx_action(tx_action),
    tx_endloop_action(tx_endloop_action)
{

}

TxSenderHandler::~TxSenderHandler()
{
    logger(debug) << "Destroying the sender handler";
}

void TxSenderHandler::setBatchSize(int batchSize)
{
    this->batch_size = batchSize;
}

int TxSenderHandler::getBatchSize() const
{
    return batch_size;
}

void TxSenderHandler::checkIfCanSend() {
    if (processed < count) {
        work_q->schedule(interval, make_work(&TxSenderHandler::checkIfCanSend, this));

        if (sndr.credit() > 0) {
            send();
        } else {
            ready = true;
        }
    }
}

void TxSenderHandler::send()
{
    logger(debug) << "[send] Preparing to send message";
    int credit = sndr.credit();

    if (credit == 0) {
        logger(warning) << "[send] There not enough credit to send messages";
    }

    logger(debug) << "[send] The handler has enough credit to send " << credit
            << " message" << (credit > 1 ? "s" : "" );

    logger(trace) << "[send] Sending messages through the link";

    message message_to_send = message(m);

    try {
        if (get<string>(message_to_send.body()).find("%d") != string::npos) {
            size_t percent_position = get<string>(message_to_send.body()).find("%d");
            stringstream ss;
            ss << processed;
            string replaced_number = get<string>(message_to_send.body()).replace(percent_position, 2, ss.str());
            message_to_send.body(replaced_number);
        }
    } catch (conversion_error &) {
    }


    logger(trace) << "[send] Transaction is empty: " << tx.is_empty();
    logger(debug) << "[send] Messages processed: " << processed;
    logger(trace) << "[send] Current batch: " << current_batch;
    while (!tx.is_empty() && sndr.credit() && (processed + current_batch) < count)
    {
        tx.send(sndr, message_to_send);
        current_batch += 1;

        if (log_msgs == "dict") {
            ReactorDecoder decoder = ReactorDecoder(message_to_send);

            std::ostringstream stream;
            DictWriter writer = DictWriter(&stream);

            DictFormatter formatter = DictFormatter();
            formatter.printMessage(&decoder, &writer);

            writer.endLine();
            std::cout << writer.toString();
        } else if (log_msgs == "interop") {
            DictFormatter formatter = DictFormatter();

            formatter.printMessageInterop(message_to_send);
        }

        if (duration_time > 0 && duration_mode == "after-send-tx-action") {
            // TODO: Not implemented yet
        }

        logger(debug) << "[send] Messages processed: " << processed;
        logger(debug) << "[send] Current batch: " << current_batch;

        if(current_batch == batch_size) {
            logger(debug) << "[send] Transaction attempt: " << tx_action;
            if (tx_action == "commit") {
                tx.commit();
            } else if (tx_action == "rollback") {
                tx.abort();
            }
            tx = transaction();

            if (tx_action == "none") {
               if (processed + current_batch == count) {
                   sndr.connection().close();
               } else {
                   processed += current_batch;
                   current_batch = 0;
                   sess.declare_transaction(*this);
               }
            }
        } else if (processed + current_batch == count) {
            logger(debug) << "[send] Transaction attempt (endloop): " << tx_endloop_action;
            if (tx_endloop_action == "commit") {
                tx.commit();
            } else if (tx_endloop_action == "rollback") {
                tx.abort();
            }
            sndr.connection().close();
        }
    }

#if defined(__REACTOR_HAS_TIMER)
    timer.reset();
#endif
    ready = false;
}

// reactor methods

void TxSenderHandler::on_sendable(sender &s)
{
    logger(trace) <<  "[on_sendable] transaction: " << &tx;
    if (ready) {
        send();
    }
}

void TxSenderHandler::on_tracker_accept(tracker &t)
{
    logger(trace) << "[on_tracker_accept] Message accepted, confirmed message delivery: " << processed;
}

void TxSenderHandler::on_connection_close(connection &c)
{
    current_batch = 0;
    logger(debug) << "[on_connection_close] Closing connection";
}

void TxSenderHandler::on_transaction_declared(transaction t) {
    logger(trace) << "[on_transaction_declared] txn called " << (&t);
    logger(trace) << "[on_transaction_declared] txn is_empty " << (t.is_empty())
                  << "\t" << tx.is_empty();
    tx = t;
    send();
}

void TxSenderHandler::on_transaction_committed(transaction t) {
    logger(trace) << "[on_transaction_committed] Messages committed";
    processed += current_batch;
    logger(debug) << "[on_transaction_committed] Messages processed" << processed;
    if (processed == count) {
        logger(trace) << "[on_transaction_committed] All messages processed";
        t.connection().close();
    } else {
        logger(trace) << "[on_transaction_committed] Declaring new transaction";
        current_batch = 0;
        sess.declare_transaction(*this);
    }
}

void TxSenderHandler::on_transaction_aborted(transaction t) {
    logger(trace) << "[on_transaction_aborted] Messages aborted";
    processed += current_batch;
    logger(debug) << "[on_transaction_committed] Messages processed" << processed;
    if (processed == count) {
        logger(trace) << "[on_transaction_aborted] All messages processed";
        t.connection().close();
    } else {
        logger(trace) << "[on_transaction_committed] Declaring new transaction";
        current_batch = 0;
        sess.declare_transaction(*this);
    }
}

void TxSenderHandler::on_sender_close(sender &s) {
    current_batch = 0;
}

void TxSenderHandler::on_session_open(session &s) {
     sess = s;
     logger(trace) << "[on_session_open] declare_txn started...";
     s.declare_transaction(*this);
     logger(trace) << "[on_session_open] declare_txn ended...";
}

void TxSenderHandler::on_container_start(container &c)
{
    logger(debug) << "[on_container_start] Starting messaging transaction handler";
    logger(debug) << "[on_container_start] User: " << user;
    logger(debug) << "[on_container_start] Password: " << password;
    logger(debug) << "[on_container_start] SASL mechanisms: " << sasl_mechanisms;
    logger(debug) << "[on_container_start] SASL enabled: " << conn_sasl_enabled;
    logger(debug) << "[on_container_start] Maximum frame size: " << max_frame_size;
    logger(debug) << "[on_container_start] Topic: " << is_topic;
    logger(debug) << "[on_container_start] Transaction batch size: " << batch_size;
    logger(debug) << "[on_container_start] Transaction action: " << tx_action;
    logger(debug) << "[on_container_start] Transaction endloop action: " << tx_endloop_action;
    logger(trace) << "[on_container_start] Messages count: " << count;
    logger(debug) << "[on_container_start] Messages processed: " << processed;

    std::vector< ::proton::symbol > caps;

    if (is_topic) {
        caps.push_back("topic");
    }

    logger(debug) << "[on_container_start] Source capabilities: ";
    for (std::vector< ::proton::symbol >::const_iterator i = caps.begin(); i != caps.end(); ++i) {
        logger(debug) << *i;
    }
    connection_options conn_opts;

    if (!user.empty()) conn_opts.user(user);
    if (!password.empty()) conn_opts.password(password);

    if (conn_sasl_enabled == "false") {
        conn_opts.sasl_enabled(false);
    } else {
        conn_opts.sasl_enabled(true);
    }

    conn_opts.sasl_allow_insecure_mechs(true);
    conn_opts.sasl_allowed_mechs(sasl_mechanisms);
    // conn_opts.max_frame_size(max_frame_size);
    conn_opts.failover_urls(conn_urls);

    logger(debug) << "[on_container_start] Setting a reconnect timer: " << conn_reconnect;
    logger(debug) << "[on_container_start] Custom reconnect: " << conn_reconnect_custom;

    configure_reconnect(conn_opts);
    configure_ssl(c);

    if (conn_heartbeat != 0) {
        logger(debug) << "[on_container_start] Heartbeat: " << conn_heartbeat;

        duration heartbeat_seconds = conn_heartbeat * duration::SECOND;

        conn_opts.idle_timeout(heartbeat_seconds);
    }

    logger(debug) << "[on_container_start] Creating a sender";

    connection conn;
    if (conn_use_config_file) {
        conn = c.connect();
    } else {
        conn = c.connect(broker_url.getUri(), conn_opts);
    }

    sndr = conn.open_sender(
            broker_url.getPath(),
            c.sender_options()
                .source(
                    source_options().capabilities(caps)
                )
    );

    work_q = &sndr.work_queue();

    logger(trace) << "[on_container_start] Setting up timer";

    if (duration_time > 0 && count > 0) {
        interval = duration((duration_time * duration::SECOND) / count);

        logger(trace) << "[on_container_start] Interval for duration: " << interval.milliseconds() << " ms";
    }
#if defined(__REACTOR_HAS_TIMER)
    work_q->schedule(duration::IMMEDIATE, make_work(&TxSenderHandler::timerEvent, this));

    if (duration_time > 0 && duration_mode == "after-send") {
        work_q->schedule(duration::IMMEDIATE, make_work(&TxSenderHandler::checkIfCanSend, this));
    } else if (duration_time > 0 && duration_mode == "before-send") {
        work_q->schedule(interval, make_work(&TxSenderHandler::checkIfCanSend, this));
    } else {
        work_q->schedule(duration::IMMEDIATE, make_work(&TxSenderHandler::checkIfCanSend, this));
    }
#endif

    tx = transaction();
}

void TxSenderHandler::on_transaction_declare_failed(transaction) {}

void TxSenderHandler::on_transaction_commit_failed(transaction t) {
    logger(error) << "[on_transaction_commit_failed] Transaction Commit Failed";
    t.connection().close();
    exit(1);
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
