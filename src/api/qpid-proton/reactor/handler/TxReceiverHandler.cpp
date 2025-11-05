/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TxReceiverHandler.cpp
 * Author: pematous
 *
 * Created on October 20, 2024
 */

#include <sstream>

#include <proton/reconnect_options.hpp>

#include "TxReceiverHandler.h"

using namespace dtests::common;
using namespace dtests::common::log;
using namespace dtests::proton::reactor;

TxReceiverHandler::TxReceiverHandler(
    const string &url,
    vector<string> conn_urls,
    bool is_topic,
    bool durable_subscriber,
    bool subscriber_unsubscribe,
    string durable_subscriber_prefix,
    string durable_subscriber_name,
    bool shared_subscriber,
    string msg_action,
    int msg_action_size,
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
    int count,
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
    bool process_reply_to,
    bool browse,
    string recv_listen,
    int recv_listen_port,
    int recv_credit_window,
    bool recv_drain_after_credit_window,
    string tx_action,
    string tx_endloop_action
)
    : super(
        url,
        conn_urls,
        is_topic,
        durable_subscriber,
        subscriber_unsubscribe,
        durable_subscriber_prefix,
        durable_subscriber_name,
        shared_subscriber,
        msg_action,
        msg_action_size,
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
        count,
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
        log_msgs,
        process_reply_to,
        browse,
        recv_listen,
        recv_listen_port,
        recv_credit_window,
        recv_drain_after_credit_window
    ),
    tx_action(tx_action),
    tx_endloop_action(tx_endloop_action)
{
}

TxReceiverHandler::~TxReceiverHandler()
{
}

void TxReceiverHandler::setBatchSize(int batchSize)
{
    this->batch_size = batchSize;
}

int TxReceiverHandler::getBatchSize() const
{
    return batch_size;
}

// reactor methods

void TxReceiverHandler::on_session_open(session &s) {
    if(!s.transaction_is_declared()) {
        logger(trace) << "[on_session_open] New session is open";
        s.transaction_declare(*this);
    } else {
        logger(trace) << "[on_session_open] Transaction is declared: " << s.transaction_id();
        if (count != 0 && (processed + batch_size > count)) {
           batch_size = count % batch_size;
        } else if (count == 0) {
            batch_size = 1;
        }
        recv.add_credit(batch_size);
        logger(debug) << "[on_session_open] Receiver credit: " << recv.credit();
    }
}

void TxReceiverHandler::on_session_transaction_commit_failed(session &s) {
    logger(debug) << "[on_session_transaction_commit_failed] Transaction Commit Failed";
    s.connection().close();
    exit(-1);
}

void TxReceiverHandler::on_session_transaction_aborted(session &s) {
    processed += current_batch;
    current_batch = 0;
    logger(debug) << "[on_session_transaction_aborted] messages aborted, processed: " << processed;
    if (count == 0 || processed < count) {
        logger(info) << "[on_session_transaction_aborted] re-declaring transaction";
        s.transaction_declare(*this);
    } else {
        logger(info) << "[on_session_transaction_aborted] All messages processed";
        s.connection().close();
    }
}

void TxReceiverHandler::on_session_transaction_committed(session &s) {
    processed += current_batch;
    current_batch = 0;
    logger(debug) << "[on_session_transaction_committed] messages committed, processed: " << processed;
    if (count == 0 || processed < count) {
        logger(info) << "[on_session_transaction_committed] re-declaring transaction";
        s.transaction_declare(*this);
    } else {
        logger(info) << "[on_session_transaction_committed] All messages processed";
        s.connection().close();
    }
}

void TxReceiverHandler::on_container_start(container &c)
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
    logger(debug) << "[on_container_start] Peer to Peer: " << recv_listen;

    if (recv_listen == "true") {
        cont = &c;
    }

    connection_options conn_opts;
    std::vector< ::proton::symbol > caps;

    if (is_topic) {
        caps.push_back("topic");

        if (durable_subscriber || subscriber_unsubscribe) {
            createSubscriptionName(durable_subscriber_prefix);
        }

        if (shared_subscriber) {
            caps.push_back("shared");
            caps.push_back("global");
        }
    }

    logger(debug) << "[on_container_start] Source capabilities: ";
    for (std::vector< ::proton::symbol >::const_iterator i = caps.begin(); i != caps.end(); ++i) {
        logger(debug) << *i;
    }

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

    logger(debug) << "[on_container_start] Browsing: " << browse;

    if (browse) {
        logger(debug) << "[on_container_start] Creating a receiver and connecting to the server";

        source_options s_opts = source_options()
            .distribution_mode(source::COPY)
            .filters(this->fm)
            .capabilities(caps);

        if (durable_subscriber || subscriber_unsubscribe) {
            s_opts.durability_mode( ::proton::source::UNSETTLED_STATE );
            s_opts.expiry_policy( ::proton::source::NEVER );
        }

        receiver_options r_opts = c.receiver_options()
            .source(
                s_opts
            );

        if (duration_time > 0 && recv_credit_window == -1) {
            r_opts.credit_window(1);
        } else if (recv_credit_window != -1) {
            r_opts.credit_window(recv_credit_window);
        }

        if (durable_subscriber || subscriber_unsubscribe) {
            r_opts.name(durable_subscriber_name);
        }

        connection conn;
        if (conn_use_config_file) {
            conn = c.connect();
        } else {
            conn = c.connect(broker_url.getUri(), conn_opts);
        }

        recv = conn.open_receiver(
                broker_url.getPath(),
                r_opts
        );

        work_q = &recv.work_queue();
    } else {
        logger(debug) << "[on_container_start] Peer-to-peer: " << recv_listen;
        logger(debug) << "[on_container_start] Peer-to-peer port: " << recv_listen_port;

        if (recv_listen == "true") {
            logger(debug) << "[on_container_start] Creating a listener";
            // P2P
            stringstream ss;
            ss << "0.0.0.0:";
            ss << recv_listen_port;
            lsnr = c.listen(ss.str(), conn_opts);
        } else {
            logger(debug) << "[on_container_start] Creating a receiver and connecting to the server";

            source_options s_opts = source_options().filters(this->fm).capabilities(caps);

            if (durable_subscriber || subscriber_unsubscribe) {
                s_opts.durability_mode( ::proton::source::UNSETTLED_STATE );
                s_opts.expiry_policy( ::proton::source::NEVER );
            }

            receiver_options r_opts = c.receiver_options()
                .source(
                    s_opts
                );

            if (durable_subscriber || subscriber_unsubscribe) {
                r_opts.name(durable_subscriber_name);
            }

            if (duration_time > 0 && recv_credit_window == -1) {
                r_opts.credit_window(1);
            } else if (recv_credit_window != -1) {
                r_opts.credit_window(recv_credit_window);
            }

            connection conn;
            if (conn_use_config_file) {
                conn = c.connect();
            } else {
                conn = c.connect(broker_url.getUri(), conn_opts);
            }

            recv = conn.open_receiver(
                    broker_url.getPath(),
                    r_opts
            );

            work_q = &recv.work_queue();
        }
    }
    logger(debug) << "[on_container_start] Connected to the broker/p2p and waiting for messages";

    if (subscriber_unsubscribe && durable_subscriber_name != "") {
        recv.close();
        recv.connection().close();
    } else {
        ts = get_time();
#if defined(__REACTOR_HAS_TIMER)
        if (recv_listen != "true") {
            work_q->schedule(duration::IMMEDIATE, make_work(&TxReceiverHandler::timerEvent, this));
        } else {
            cont->schedule(duration::IMMEDIATE, make_work(&TxReceiverHandler::timerEvent, this));
        }
#endif
    }

}

void TxReceiverHandler::on_message(delivery &d, message &m)
{
    logger(debug) << "[on_message] Processing received message";

    session s = d.session();
    d.accept();
    current_batch += 1;

    if (log_msgs == "dict") {
        logger(trace) << "[on_message] Decoding message";
        ReactorDecoder decoder = ReactorDecoder(m);

        std::ostringstream stream;
        DictWriter writer = DictWriter(&stream);

        DictFormatter formatter = DictFormatter();
        formatter.printMessage(&decoder, &writer);

        writer.endLine();
        std::cout << writer.toString();
    } else if (log_msgs == "interop") {
        DictFormatter formatter = DictFormatter();

        formatter.printMessageInterop(m);
    }

    if (duration_time > 0 && duration_mode == "after-receive") {
        logger(debug) << "[on_message] Waiting...";
        sleep4next(ts, count, duration_time, processed + current_batch);
    }

    if(((processed + current_batch) % msg_action_size) == 0) {
        do_message_action(d);
    }

    if (duration_time > 0 && duration_mode == "after-receive-action") {
        sleep4next(ts, count, duration_time, processed + current_batch);
    }

    logger(debug) << "[on_message] Process-reply-to: " << process_reply_to;

    if (process_reply_to) {
        if (m.reply_to() != "") {
            logger(debug) << "[on_message] Reply-to address: " << m.reply_to();

            do_process_reply_to(m);
        } else {
            logger(debug) << "[on_message] Reply-to address is not set";
        }
    }

    if (recv_drain_after_credit_window && processed + current_batch == recv_credit_window) {
        logger(debug) << "[on_message] Scheduling drain";
        d.receiver().work_queue().add(make_work(&TxReceiverHandler::drain, this));
    }

    if (!process_reply_to && processed + current_batch == count) {
        if (durable_subscriber) {
            d.receiver().detach();
        } else {
            d.receiver().close();
        }
        d.connection().close();
    } else {
#if defined(__REACTOR_HAS_TIMER)
        super::timer.reset();
#endif
    }

    logger(debug) << "[on_message] Receiver credit: " << recv.credit();
    logger(debug) << "[on_message] Current batch: " << current_batch;
    if(current_batch == batch_size) {
        logger(debug) << "[on_message] Transaction attempt: " << tx_action;
        if (tx_action == "commit") {
            s.transaction_commit();
        } else if (tx_action == "rollback") {
            s.transaction_abort();
        }

        if (tx_action == "none") {
           if (processed + current_batch == count) {
               recv.connection().close();
           } else {
               processed += current_batch;
               current_batch = 0;
               s.transaction_declare(*this);
           }
        }

        if (duration_time > 0 && duration_mode == "after-receive-action-tx-action") {
            // TODO: not implemented yet
        }

    } else if (count != 0 && processed + current_batch == count) {
        logger(debug) << "[on_message] Transaction attempt (endloop): " << tx_endloop_action;
        if (tx_endloop_action == "commit") {
            s.transaction_commit();
        } else if (tx_endloop_action == "rollback") {
            s.transaction_abort();
        } else {
          recv.connection().close();
        }
    }
}

void TxReceiverHandler::on_transport_close(transport &t) {
    logger(debug) << "[on_transport_close] Closing the transport";
    current_batch = 0;
    if (conn_reconnect == "false") {
        exit(1);
    } else if (processed == count) {
        exit(0);
    }
}
