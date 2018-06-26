/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReceiverHandler.cpp
 * Author: opiske
 * 
 * Created on October 19, 2015, 4:43 PM
 */

#include <sstream>

#include <proton/reconnect_options.hpp>

#include "ReceiverHandler.h"

using namespace dtests::common;
using namespace dtests::common::log;
using namespace dtests::proton::reactor;

ReceiverHandler::ReceiverHandler(
    const string &url,
    vector<string> failover_urls,
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
    string log_msgs,
    bool process_reply_to,
    bool browse,
    string recv_listen,
    int recv_listen_port
)
    : super(
        url,
        failover_urls,
        is_topic,
        user,
        password,
        sasl_mechanisms,
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
        log_msgs
    ),
    durable_subscriber(durable_subscriber),
    subscriber_unsubscribe(subscriber_unsubscribe),
    durable_subscriber_prefix(durable_subscriber_prefix),
    durable_subscriber_name(durable_subscriber_name),
    shared_subscriber(shared_subscriber),
    interval(timeout * duration::SECOND),
    timer_event(*this),
    msg_action(msg_action),
    msg_action_size(msg_action_size),
    msg_received_cnt(0),
    process_reply_to(process_reply_to),
    browse(browse),
    count(count),
    duration_time(duration_time),
    duration_mode(duration_mode),
    recv_listen(recv_listen),
    recv_listen_port(recv_listen_port)
{
}

ReceiverHandler::~ReceiverHandler()
{
}

void ReceiverHandler::timerEvent() {
#if defined(__REACTOR_HAS_TIMER)
    if (timer.isExpired()) {
        logger(info) << "Timed out";

        if (recv_listen != "true") {
            exit(EXIT_SUCCESS);
        } else {
            exit(0);
        }
    } else {
        timer--;
        logger(debug) << "Waiting ...";
        
        if (recv_listen != "true") {
            work_q->schedule(duration::SECOND, make_work(&ReceiverHandler::timerEvent, this));
        } else {
            cont->schedule(duration::SECOND, make_work(&ReceiverHandler::timerEvent, this));
        }
    }
#endif
}

void ReceiverHandler::on_container_start(container &c)
{
    logger(debug) << "Starting messaging handler";

    if (recv_listen == "true") {
        cont = &c;
    }

    logger(debug) << "User: " << user;
    logger(debug) << "Password: " << password;
    logger(debug) << "SASL mechanisms: " << sasl_mechanisms;
    
    logger(debug) << "Maximum frame size: " << max_frame_size;

    logger(debug) << "Topic: " << is_topic;

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

    logger(debug) << "Source capabilities: ";
    for (std::vector< ::proton::symbol >::const_iterator i = caps.begin(); i != caps.end(); ++i) {
        logger(debug) << *i;
    }

    if (!user.empty()) conn_opts.user(user);
    if (!password.empty()) conn_opts.password(password);

    conn_opts.sasl_enabled(true);
    conn_opts.sasl_allow_insecure_mechs(true);
    conn_opts.sasl_allowed_mechs(sasl_mechanisms);
    // conn_opts.max_frame_size(max_frame_size);

    logger(debug) << "Setting a reconnect timer: " << conn_reconnect;
    logger(debug) << "Custom reconnect: " << conn_reconnect_custom;
    
    configure_reconnect(conn_opts);

    if (conn_heartbeat != 0) {
        logger(debug) << "Heartbeat: " << conn_heartbeat;

        duration heartbeat_seconds = conn_heartbeat * duration::SECOND;

        conn_opts.idle_timeout(heartbeat_seconds);
    }

    logger(debug) << "Browsing: " << browse;

    if (browse) {
        logger(debug) << "Creating a receiver and connecting to the server";

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

        if (duration_time > 0) {
            r_opts.credit_window(1);
        }

        if (durable_subscriber || subscriber_unsubscribe) {
            r_opts.name(durable_subscriber_name);
        }

        connection conn = c.connect(broker_url.getUri(), conn_opts);

        recv = conn.open_receiver(
                broker_url.getPath(),
                r_opts
        );

        work_q = &recv.work_queue();
    } else {
        logger(debug) << "Peer-to-peer: " << recv_listen;
        logger(debug) << "Peer-to-peer port: " << recv_listen_port;

        if (recv_listen == "true") {
            logger(debug) << "Creating a listener";
            // P2P
            stringstream ss;
            ss << "0.0.0.0:";
            ss << recv_listen_port;
            lsnr = c.listen(ss.str(), conn_opts);
        } else {
            logger(debug) << "Creating a receiver and connecting to the server";

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

            if (duration_time > 0) {
                r_opts.credit_window(1);
            }

            connection conn = c.connect(broker_url.getUri(), conn_opts);

            recv = conn.open_receiver(
                    broker_url.getPath(),
                    r_opts
            );

            work_q = &recv.work_queue();
        }
    }
    logger(debug) << "Connected to the broker/p2p and waiting for messages";

    if (subscriber_unsubscribe && durable_subscriber_name != "") {
        recv.close();
        recv.connection().close();
    } else {
        ts = get_time();
#if defined(__REACTOR_HAS_TIMER)
        if (recv_listen != "true") {
            work_q->schedule(duration::IMMEDIATE, make_work(&ReceiverHandler::timerEvent, this));
        } else {
            cont->schedule(duration::IMMEDIATE, make_work(&ReceiverHandler::timerEvent, this));
        }
#endif
    }

}

void ReceiverHandler::do_message_action(delivery &d)
{
    logger(debug) << "Message action: " << msg_action;
    logger(debug) << "Message action size: " << msg_action_size;

    if(msg_action == "ack") {
        d.accept();

        logger(debug) << "Message acknowledged";
    } else if(msg_action == "reject") {
        d.reject();

        logger(debug) << "Message rejected";
    } else if(msg_action == "release") {
        d.release();

        logger(debug) << "Message released";
    }
}

void ReceiverHandler::do_process_reply_to(message &m)
{
    logger(debug) << "Processing reply-to";

    map<string, sender>::iterator it = senders.find(m.reply_to());

    if (it != senders.end()) {
        logger(debug) << "Sender for " << m.reply_to() << " found";
    } else {
        logger(debug) << "Sender for " << m.reply_to() << " not found";
        logger(debug) << "Creating sender for " << m.reply_to();

        senders[m.reply_to()] = recv.connection().open_sender(m.reply_to());

        logger(debug) << "Sender for " << m.reply_to() << " created";
    }

    message replyToMessage = message(m);
    replyToMessage.to(m.reply_to());

    logger(debug) << "Sending reply to " << replyToMessage.to();

    senders[m.reply_to()].send(replyToMessage);
}

void ReceiverHandler::on_message(delivery &d, message &m)
{
    msg_received_cnt += 1;

    logger(debug) << "Processing received message";

    if (log_msgs == "dict") {
        logger(trace) << "Decoding message";
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
        logger(debug) << "Waiting...";
        sleep4next(ts, count, duration_time, msg_received_cnt);
    }

    if((msg_received_cnt % msg_action_size) == 0) {
        do_message_action(d);
    }

    if (duration_time > 0 && duration_mode == "after-receive-action") {
        sleep4next(ts, count, duration_time, msg_received_cnt);
    }

    if (duration_time > 0 && duration_mode == "after-receive-action-tx-action") {
        // TODO: Transactions are not supported yet
    }

    logger(debug) << "Process-reply-to: " << process_reply_to;

    if (process_reply_to) {
        if (m.reply_to() != "") {
            logger(debug) << "Reply-to address: " << m.reply_to();

            do_process_reply_to(m);
        } else {
            logger(debug) << "Reply-to address is not set";
        }
    }

    if (!process_reply_to && msg_received_cnt == count) {
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
}


void ReceiverHandler::on_tracker_accept(tracker &t)
{
    logger(debug) << "Delivery accepted";
}


void ReceiverHandler::on_tracker_reject(tracker &t)
{
    logger(debug) << "Delivery rejected";
}

void ReceiverHandler::on_transport_close(transport &t) {
    logger(debug) << "Closing the transport";

    if (conn_reconnect == "false") {
        exit(1);
    } else if (msg_received_cnt == count) {
        exit(0);
    }
}

void ReceiverHandler::on_transport_error(transport &t) {
    logger(error) << "The connection with " << broker_url.getHost() << ":" << broker_url.getPort() << " was interrupted: " << t.error().what();
    
    if (t.error().what().find("unauthorized") != string::npos) {
        exit(1);
    }
}

void ReceiverHandler::on_connection_close(connection &conn)
{
    logger(debug) << "Disconnecting ...";
}

void ReceiverHandler::on_connection_error(connection &c)
{
    logger(error) << "Failed to connect to " << broker_url.getHost() << ":" << broker_url.getPort();

    if (c.error().what().find("Unable to validate user") != string::npos) {
        exit(1);
    }
}

void ReceiverHandler::do_disconnect()
{

}

void ReceiverHandler::setSelector(string selector)
{
    source::filter_map map;
    symbol filter_key("selector");
    value filter_value;
    encoder enc(filter_value);

    enc << start::described()
        << symbol("apache.org:selector-filter:string")
        << selector
        << finish();
    
    fm.put(filter_key, filter_value);
}

void ReceiverHandler::createSubscriptionName(string customPrefix)
{
    if (durable_subscriber_name == "") {
        uuid client_uuid = ::proton::uuid::random();

        if (customPrefix == "") {
            durable_subscriber_name = "qpid-proton-cpp-" + client_uuid.str();
        } else {
            durable_subscriber_name = customPrefix + client_uuid.str();
        }
    }

    logger(debug) << "Durable subscription name: " << durable_subscriber_name;
}
