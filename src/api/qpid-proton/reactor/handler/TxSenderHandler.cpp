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
    ready(false),
    count(1),
    duration_time(duration_time),
    duration_mode(duration_mode),
    batch_size(1),
    current_batch(0),
    committed(0),
    confirmedSent(0),
    tx_action(tx_action),
    tx_endloop_action(tx_endloop_action),
    // TODO whats this ?
    m(),
    timer_event(*this),
    interval(duration::IMMEDIATE)
{

}

TxSenderHandler::~TxSenderHandler()
{
    logger(debug) << "Destroying the sender handler";
}

void TxSenderHandler::timerEvent() {
#if defined(__REACTOR_HAS_TIMER)
    if (timer.isExpired()) {
        logger(info) << "Timed out";

        exit(EXIT_SUCCESS);
    } else {
        timer--;
        logger(debug) << "Waiting ...";

        work_q->schedule(duration::SECOND, make_work(&TxSenderHandler::timerEvent, this));
    }
#endif
}

void TxSenderHandler::setCount(int count)
{
    this->count = count;
}

int TxSenderHandler::getCount() const
{
    return count;
}

void TxSenderHandler::setBatchSize(int batchSize)
{
    this->batch_size = batchSize;
}

int TxSenderHandler::getBatchSize() const
{
    return batch_size;
}

void TxSenderHandler::setMessage(message &msg)
{
    this->m = msg;
}

message TxSenderHandler::getMessage() const
{
    return m;
}

void TxSenderHandler::checkIfCanSend() {
    if (confirmedSent < count) {
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
    logger(debug) << "Preparing to send message";
    int credit = sndr.credit();

    if (credit == 0) {
        logger(warning) << "There not enough credit to send messages";
    }

    logger(debug) << "The handler has enough credit to send " << credit
            << " message" << (credit > 1 ? "s" : "" );

    logger(trace) << "Sending messages through the link";

    message message_to_send = message(m);

    try {
        if (get<string>(message_to_send.body()).find("%d") != string::npos) {
            size_t percent_position = get<string>(message_to_send.body()).find("%d");
            stringstream ss;
            ss << confirmedSent;
            string replaced_number = get<string>(message_to_send.body()).replace(percent_position, 2, ss.str());
            message_to_send.body(replaced_number);
        }
    } catch (conversion_error &) {
    }

    logger(trace) << "Transaction variables";
    logger(trace) << "confirmed: " << confirmedSent;
    logger(trace) << "commited: " << committed;
    logger(trace) << "count: " << count;
    logger(trace) << "is_empty: " << tx.is_empty();
    logger(trace) << "current_batch: " << current_batch;

    while (!tx.is_empty() && sndr.credit() && (committed + current_batch) < count)
    {
        tx.send(sndr, message_to_send);

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
            // TODO: Transactions are not supported yet
        }

        confirmedSent++;

        current_batch += 1;

        logger(trace) << "Transaction variables";
        logger(trace) << "confirmed: " << confirmedSent;
        logger(trace) << "commited: " << committed;
        logger(trace) << "count: " << count;
        logger(trace) << "is_empty: " << tx.is_empty();
        logger(trace) << "current_batch: " << current_batch;

        if (confirmedSent + current_batch == count) {
            logger(debug) << "[send] Transaction attempt (endloop): " << tx_endloop_action;
            if (tx_endloop_action == "commit") {
                tx.commit();
            } else if (tx_endloop_action == "rollback") {
                tx.abort();
            }
            sndr.connection().close();
        } else if(current_batch == batch_size) {
            logger(debug) << "[send] Transaction attempt: " << tx_action;
            if (tx_endloop_action == "commit") {
                tx.commit();
            } else if (tx_endloop_action == "rollback") {
                tx.abort();
            }
            tx = transaction();
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
    logger(trace) << "[on_tracker_accept] Message accepted. Now obtaining the connection reference object";
    connection conn = t.connection();

    confirmedSent += 1;
    logger(trace) << "[on_tracker_accept] Confirmed message delivery " << confirmedSent;
}

void TxSenderHandler::on_tracker_reject(tracker &t)
{
    std::cerr << "[error] Delivery rejected" << std::endl;
    exit(1);
}

void TxSenderHandler::on_transport_error(transport &t) {
    logger(error) << "The connection with " << broker_url.getHost() << ":" << broker_url.getPort() << " was interrupted: " << t.error().what();

    if (t.error().what().find("unauthorized") != string::npos) {
        exit(1);
    }
}

void TxSenderHandler::on_transport_close(transport &t) {
    logger(debug) << "Closing the transport";

    if (conn_reconnect == "false") {
        exit(1);
    }
}

void TxSenderHandler::on_connection_close(connection &c)
{
    logger(debug) << "Closing connection";
    logger(info) << "Transactions status";
    logger(info) << "Transaction batch size: " << batch_size;
    logger(info) << "Transaction current batch: " << current_batch;
    logger(info) << "Transaction confirmed: " << confirmedSent;
    logger(info) << "Transaction committed: " << committed;
    current_batch = 0;
}

void TxSenderHandler::on_connection_error(connection &c)
{
    logger(error) << "Failed to connect to " << broker_url.getHost() << ":" << broker_url.getPort();

    if (c.error().what().find("Unable to validate user") != string::npos) {
        exit(1);
    }
}

void TxSenderHandler::on_transaction_declared(transaction t) {
    logger(trace) << "[on_transaction_declared] txn called " << (&t);
    tx = t;
    send();
    logger(trace) << "[on_transaction_declared] txn is_empty " << (t.is_empty())
                  << "\t" << tx.is_empty();
}

void TxSenderHandler::on_transaction_committed(transaction t) {
    logger(trace) << "[on_transaction_commited] Messages committed";
    committed += current_batch;
    current_batch = 0;
    connection conn = sndr.connection();
    if(committed == count) {
        logger(trace) << "[on_transaction_commited] All messages committed";
        conn.close();
    }
    else {
        cont->declare_transaction(conn, *this);
    }
}

// TODO jak je to s temi override ?
// ?? python to tak ma ?? stejne jak commited
// void TxSenderHandler::on_transaction_aborted(transaction t) override {
void TxSenderHandler::on_transaction_aborted(transaction t) {
    logger(debug) << "[on_transaction_aborted] Mesages Aborted";
    current_batch = 0;
    cont->declare_transaction(sndr.connection(), *this);
}

void TxSenderHandler::on_sender_close(sender &s) {
    current_batch = 0;
}

void TxSenderHandler::on_container_start(container &c) 
{
    logger(debug) << "Starting messaging handler";

    logger(debug) << "User: " << user;
    logger(debug) << "Password: " << password;
    logger(debug) << "SASL mechanisms: " << sasl_mechanisms;
    logger(debug) << "SASL enabled: " << conn_sasl_enabled;

    logger(debug) << "Maximum frame size: " << max_frame_size;

    logger(info) << "Transaction batch size" << batch_size;

    logger(debug) << "Topic: " << is_topic;

    std::vector< ::proton::symbol > caps;

    if (is_topic) {
        caps.push_back("topic");
    }

    logger(debug) << "Source capabilities: ";
    for (std::vector< ::proton::symbol >::const_iterator i = caps.begin(); i != caps.end(); ++i) {
        logger(debug) << *i;
    }
//    connection_options conn_opts;
//
//    if (!user.empty()) conn_opts.user(user);
//    if (!password.empty()) conn_opts.password(password);
//
//    if (conn_sasl_enabled == "false") {
//        conn_opts.sasl_enabled(false);
//    } else {
//        conn_opts.sasl_enabled(true);
//    }
//
//    conn_opts.sasl_allow_insecure_mechs(true);
//    conn_opts.sasl_allowed_mechs(sasl_mechanisms);
//    // conn_opts.max_frame_size(max_frame_size);
//    conn_opts.failover_urls(conn_urls);
//
//    logger(debug) << "Setting a reconnect timer: " << conn_reconnect;
//    logger(debug) << "Custom reconnect: " << conn_reconnect_custom;
//
//    configure_reconnect(conn_opts);
//    configure_ssl(c);
//
//    if (conn_heartbeat != 0) {
//        logger(debug) << "Heartbeat: " << conn_heartbeat;
//
//        duration heartbeat_seconds = conn_heartbeat * duration::SECOND;
//
//        conn_opts.idle_timeout(heartbeat_seconds);
//    }
//
//    logger(debug) << "Creating a sender";
//
//    connection conn;
//    if (conn_use_config_file) {
//        conn = c.connect();
//    } else {
//        conn = c.connect(broker_url.getUri(), conn_opts);
//    }

    connection conn = c.connect(broker_url.getUri());
    sndr = conn.open_sender(
            broker_url.getPath(),
            c.sender_options()
                .source(
                    source_options().capabilities(caps)
                )
    );

//    work_q = &sndr.work_queue();
//
//    logger(trace) << "Setting up timer";
//
//    if (duration_time > 0 && count > 0) {
//        interval = duration((duration_time * duration::SECOND) / count);
//
//        logger(trace) << "Interval for duration: " << interval.milliseconds() << " ms";
//    }
//#if defined(__REACTOR_HAS_TIMER)
//    work_q->schedule(duration::IMMEDIATE, make_work(&TxSenderHandler::timerEvent, this));
//
//    if (duration_time > 0 && duration_mode == "after-send") {
//        work_q->schedule(duration::IMMEDIATE, make_work(&TxSenderHandler::checkIfCanSend, this));
//    } else if (duration_time > 0 && duration_mode == "before-send") {
//        work_q->schedule(interval, make_work(&TxSenderHandler::checkIfCanSend, this));
//    } else {
//        work_q->schedule(duration::IMMEDIATE, make_work(&TxSenderHandler::checkIfCanSend, this));
//    }
//#endif

    tx = transaction();
    logger(debug) << "[on_container_start] declare_txn started...";
    c.declare_transaction(conn, *this);
    cont = &c;
    logger(debug) << "[on_container_start] completed (container:" << &c << ", transaction: " << &tx << ")";
}

void TxSenderHandler::on_transaction_declare_failed(transaction) {}

void TxSenderHandler::on_transaction_commit_failed(transaction) {
    logger(error) << "[on_transaction_commit_failed] Transaction Commit Failed";
    sndr.connection().close();
    exit(1);
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
