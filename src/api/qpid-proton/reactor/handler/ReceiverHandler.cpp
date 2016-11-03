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

ReceiverHandler::ReceiverHandler(const string &url, string msg_action, string user, string password, string sasl_mechanisms, int timeout)
    : super(url, user, password, sasl_mechanisms, timeout),
    interval(timeout * duration::SECOND.milliseconds()),
    msg_action(msg_action),
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
    recv = c.open_receiver(
            broker_url,
            c.client_connection_options()
                .user(user)
                .password(password)
                .sasl_enabled(true) // TODO: CLI parameter???
                .sasl_allow_insecure_mechs(true) // TODO: CLI parameter???
                .sasl_allowed_mechs(sasl_mechanisms)
    );
    logger(debug) << "Connected to the broker and waiting for messages";

    duration d = duration(int(1000 * duration::SECOND.milliseconds()));
#if defined(__REACTOR_HAS_TIMER)
    c.schedule(d, timer_event);
#endif

}

void ReceiverHandler::do_message_action(delivery &d)
{
    logger(debug) << "Message action: " << msg_action;

    if(msg_action == "ack") {
        d.accept();

        logger(debug) << "Message acknowledged";
    } else if(msg_action == "reject") {
        d.reject();

        logger(debug) << "Message rejected";
    }
}

void ReceiverHandler::on_message(delivery &d, message &m)
{
    logger(debug) << "Processing received message";

    logger(trace) << "Decoding message";
    ReactorDecoder decoder = ReactorDecoder(m);

    std::ostringstream stream;
    DictWriter writer = DictWriter(&stream);

    DictFormatter formatter = DictFormatter();
    formatter.printMessage(&decoder, &writer);

    writer.endLine();
    std::cout << writer.toString();

    do_message_action(d);

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

