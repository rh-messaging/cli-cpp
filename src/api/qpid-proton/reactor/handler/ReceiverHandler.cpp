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

void ReceiverHandler::on_container_start(event &e, container &c)
{
    logger(debug) << "Starting messaging handler";
    
    logger(debug) << "Creating a receiver and connecting to the server";
    c.open_receiver(broker_url);
#ifdef REACTIVE_HAS_TIMER_
    super::setupTimer(e);
#endif // REACTIVE_HAS_TIMER_
}

void ReceiverHandler::on_message(event &e, message &m)
{
    logger(debug) << "Event name: " << e.name();
    
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

void ReceiverHandler::on_delivery_accept(event &e)
{
    logger(debug) << "Accepted: " << e.name();
}


void ReceiverHandler::on_delivery_reject(event &e)
{
    logger(debug) << "Event name: " << e.name();
}


void ReceiverHandler::on_connection_close(event &e)
{
    logger(debug) << "Disconnected: " << e.name();

    if (!super::timeoutTask) {
        logger(debug) << "Quiescing, therefore ignoring event: " << e.name();

        return;
    }

    logger(debug) << "Canceling scheduled tasks ";
#ifdef REACTIVE_HAS_TIMER_    
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

