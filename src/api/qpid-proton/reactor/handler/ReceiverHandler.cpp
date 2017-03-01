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

#include "ReceiverHandler.h"
#include <sstream>

using namespace dtests::common;
using namespace dtests::common::log;
using namespace dtests::proton::reactor;

ReceiverHandler::ReceiverHandler(
    const string &url,
    string msg_action,
    int msg_action_size,
    string user,
    string password,
    string sasl_mechanisms,
    int timeout,
    string conn_reconnect,
    int32_t conn_reconnect_interval,
    int32_t conn_reconnect_limit,
    int32_t conn_reconnect_timeout,
    uint32_t conn_reconnect_first,
    uint32_t conn_reconnect_increment,
    bool conn_reconnect_doubling,
    bool conn_reconnect_custom,
    uint32_t max_frame_size,
    string log_msgs,
    bool process_reply_to,
    bool browse
)
    : super(
        url,
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
        max_frame_size,
        log_msgs
    ),
    interval(timeout * duration::SECOND.milliseconds()),
    msg_action(msg_action),
    msg_action_size(msg_action_size),
    msg_received_cnt(0),
    process_reply_to(process_reply_to),
    browse(browse),
    timer_event(*this)
{
}

ReceiverHandler::~ReceiverHandler()
{
}

void ReceiverHandler::timerEvent() {
#if defined(__REACTOR_HAS_TIMER)
    if (timer.isExpired()) {
        logger(info) << "Timed out";

        recv.container().stop();
    } else {
        timer--;
        logger(debug) << "Waiting ...";
        
        duration d = duration(1 * duration::SECOND.milliseconds());
        recv.container().schedule(d, timer_event);
    }
#endif
}

void ReceiverHandler::on_container_start(container &c)
{
    logger(debug) << "Starting messaging handler";

    logger(debug) << "User: " << user;
    logger(debug) << "Password: " << password;
    logger(debug) << "SASL mechanisms: " << sasl_mechanisms;
    
    logger(debug) << "Maximum frame size: " << max_frame_size;

    connection_options conn_opts;
    
    if (!user.empty()) conn_opts.user(user);
    if (!password.empty()) conn_opts.password(password);

    conn_opts.sasl_enabled(true);
    conn_opts.sasl_allow_insecure_mechs(true);
    conn_opts.sasl_allowed_mechs(sasl_mechanisms);
    // conn_opts.max_frame_size(max_frame_size);

    logger(debug) << "Setting a reconnect timer: " << conn_reconnect;
    logger(debug) << "Custom reconnect: " << conn_reconnect_custom;
    
    if (conn_reconnect == "true" && conn_reconnect_custom == false) {
        conn_opts.reconnect(reconnect_timer());
    } else if (conn_reconnect == "true" && conn_reconnect_custom == true) {
        logger(debug) << "Reconnect first: " << conn_reconnect_first;
        logger(debug) << "Reconnect interval (max): " << conn_reconnect_interval;
        logger(debug) << "Reconnect increment: " << conn_reconnect_increment;
        logger(debug) << "Reconnect doubling: " << conn_reconnect_doubling;
        logger(debug) << "Reconnect limit (max_retries): " << conn_reconnect_limit;
        logger(debug) << "Reconnect timeout: " << conn_reconnect_timeout;

        conn_opts.reconnect(
            reconnect_timer(
                conn_reconnect_first,
                conn_reconnect_interval,
                conn_reconnect_increment,
                conn_reconnect_doubling,
                conn_reconnect_limit,
                conn_reconnect_timeout
            )
        );
    }

    logger(debug) << "Creating a receiver and connecting to the server";

    logger(debug) << "Browsing: " << browse;

    if (browse) {
        recv = c.open_receiver(
                broker_url,
                c.receiver_options()
                    .source(
                        source_options().distribution_mode(source::COPY)
                    ),
                conn_opts
        );
    } else {
        recv = c.open_receiver(
                broker_url,
                conn_opts
        );
    }
    logger(debug) << "Connected to the broker and waiting for messages";

    duration d = duration(int(1000 * duration::SECOND.milliseconds()));
#if defined(__REACTOR_HAS_TIMER)
    c.schedule(d, timer_event);
#endif

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
    }

    if((msg_received_cnt % msg_action_size) == 0) {
        do_message_action(d);
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

#if defined(__REACTOR_HAS_TIMER)
    super::timer.reset();
#endif
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
}

/*
void ReceiverHandler::on_transport_error(transport &t) {
    logger(error) << "The connection with " << broker_url.host_port() << 
            " was interrupted";
}
*/

void ReceiverHandler::on_connection_close(connection &conn)
{
    logger(debug) << "Disconnecting ...";
}

void ReceiverHandler::on_connection_error(connection &c)
{
    logger(error) << "Failed to connect to " << broker_url.host_port();
}

void ReceiverHandler::do_disconnect()
{

}

