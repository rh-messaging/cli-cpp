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

void ReceiverHandler::on_start(event &e)
{
    logger(debug) << "Starting messaging handler";
    
    logger(debug) << "Creating a receiver and connecting to the server";
    e.container().open_receiver(broker_url);
    
    super::setupTimer(e);
}

void ReceiverHandler::on_message(event &e)
{
    logger(debug) << "Event name: " << e.name();
    
    logger(trace) << "Decoding message";
    ReactorDecoder decoder = ReactorDecoder(e.message());

    std::ostringstream stream;
    DictWriter writer = DictWriter(&stream);

    DictFormatter formatter = DictFormatter();
    formatter.printMessage(&decoder, &writer);

    writer.endLine();
    std::cout << writer.toString();
    super::timer.reset();
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
    
    super::disableTimer();
}

void ReceiverHandler::on_timer(event &e)
{
   super::timerEvent(e);
   
}

void ReceiverHandler::do_disconnect()
{

}

