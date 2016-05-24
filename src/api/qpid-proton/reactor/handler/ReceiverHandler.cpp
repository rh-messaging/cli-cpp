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

ReceiverHandler::ReceiverHandler(const string &url, int timeout)
    : super(url, timeout)
{
}

ReceiverHandler::~ReceiverHandler()
{
}

void ReceiverHandler::on_container_start(container &c)
{
    logger(debug) << "Starting messaging handler";
    
    logger(debug) << "Creating a receiver and connecting to the server";
    recv = c.open_receiver(broker_url);
    logger(debug) << "Connected to the broker and waiting for messages";
#ifdef REACTIVE_HAS_TIMER_
    super::setupTimer(e);
#endif // REACTIVE_HAS_TIMER_
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
#ifdef REACTIVE_HAS_TIMER_
    super::timer.reset();
#endif // REACTIVE_HAS_TIMER_
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

#ifdef REACTIVE_HAS_TIMER_
    if (!super::timeoutTask) {
        logger(debug) << "Quiescing, therefore ignoring closing event";

        return;
    }

    logger(debug) << "Canceling scheduled tasks ";
    
    super::disableTimer();
#endif // REACTIVE_HAS_TIMER_
}

#ifdef REACTIVE_HAS_TIMER_
void ReceiverHandler::on_timer(event &e, container &c)
{
   super::timerEvent(e, c);
   
}
#endif // REACTIVE_HAS_TIMER_

void ReceiverHandler::do_disconnect()
{

}

