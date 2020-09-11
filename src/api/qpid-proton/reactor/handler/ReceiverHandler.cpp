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

ReceiverHandler::ReceiverHandler(const string &url, string msg_action, int msg_action_size, string user, string password, string sasl_mechanisms, int timeout, bool process_reply_to, bool browse)
    : super(url, user, password, sasl_mechanisms, timeout),
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

    logger(debug) << "Creating a receiver and connecting to the server";

    logger(debug) << "Browsing: " << browse;

    if (browse) {
        recv = c.open_receiver(
                broker_url,
                c.receiver_options()
                    .source(
                        source_options().distribution_mode(source::COPY)
                    ),
                c.client_connection_options()
                    .user(user)
                    .password(password)
                    .sasl_enabled(true)
                    .sasl_allow_insecure_mechs(true)
                    .sasl_allowed_mechs(sasl_mechanisms)
        );
    } else {
        recv = c.open_receiver(
                broker_url,
                c.client_connection_options()
                    .user(user)
                    .password(password)
                    .sasl_enabled(true)
                    .sasl_allow_insecure_mechs(true)
                    .sasl_allowed_mechs(sasl_mechanisms)
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

    logger(trace) << "Decoding message";
    ReactorDecoder decoder = ReactorDecoder(m);

    std::ostringstream stream;
    DictWriter writer = DictWriter(&stream);

    DictFormatter formatter = DictFormatter();
    formatter.printMessage(&decoder, &writer);

    writer.endLine();
    std::cout << writer.toString();

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


void ReceiverHandler::on_connection_close(connection &conn)
{
    logger(debug) << "Disconnecting ...";
}

void ReceiverHandler::do_disconnect()
{

}

