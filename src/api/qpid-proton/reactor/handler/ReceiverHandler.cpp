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
    : super(url),
    timeoutTask(NULL), timer(timeout, "timeout")
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

    logger(debug) << "Setting up timeout";
    task t = e.container().schedule(1000);
    timeoutTask = &t;
}

void ReceiverHandler::on_message(event &e)
{
    logger(debug) << "Message received: " << e.name();
    ReactorDecoder decoder = ReactorDecoder(e.message());

    std::ostringstream stream;
    DictWriter writer = DictWriter(&stream);

    DictFormatter formatter = DictFormatter();
    formatter.printMessage(&decoder, &writer);

    writer.endLine();
    std::cout << writer.toString();
    timer.reset();
}

void ReceiverHandler::on_delivery_accept(event &e)
{
    logger(debug) << "Accepted: " << e.name();
}

void ReceiverHandler::on_connection_close(event &e)
{
    logger(debug) << "Disconnected: " << e.name();

    if (!timeoutTask) {
        logger(debug) << "Quiescing, therefore ignoring event: " << e.name();

        return;
    }

    logger(debug) << "Canceling scheduled tasks ";
    
    
    timeoutTask->cancel();
    timeoutTask = NULL;
}

void ReceiverHandler::on_timer(event &e)
{

    if (!timeoutTask) {
        logger(debug) << "Quiescing, therefore ignoring event: " << e.name();

        return;
    }

    if (timer.isExpired()) {
        logger(info) << "Timed out";

        timeoutTask->cancel();
        timeoutTask = NULL;

        e.connection().close();
    } else {
        timer--;
        logger(debug) << "Waiting ...";
        e.container().schedule(1000);
    }
}

void ReceiverHandler::do_disconnect()
{

}

