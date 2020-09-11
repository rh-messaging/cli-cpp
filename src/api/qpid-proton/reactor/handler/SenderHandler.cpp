/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SenderHandler.cpp
 * Author: opiske
 * 
 * Created on October 16, 2015, 10:19 AM
 */

#include "SenderHandler.h"

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;
using namespace dtests::common::log;

SenderHandler::SenderHandler(const string &url, int timeout)
    : super(url),
    count(1),
    sent(0),
    confirmedSent(0),
    timeoutTask(NULL), 
    timer(timeout, "timeout"),
    m()
{


}

SenderHandler::~SenderHandler()
{
    logger(debug) << "Destroying the sender handler";
}

void SenderHandler::on_start(event &e)
{
    logger(debug) << "Starting messaging handler";
    
    logger(trace) << "Setting up timer";
    task t = e.container().schedule(500);
    timeoutTask = &t;

    logger(debug) << "Creating a sender";
    e.container().open_sender(broker_url);
}

void SenderHandler::on_sendable(event &e)
{
    sender s = e.sender();
    logger(debug) << "Event name: " << e.name();
    int credit = s.credit();

    if (credit == 0) {
        logger(warning) << "There not enough credit to send messages";
    }

    logger(debug) << "The handler has enough credit to send " << credit
            << " message" << (credit > 1 ? "s" : "" );
    logger(debug) << "The handler has sent " << sent << " messages"  
            << (sent > 1 ? "s" : "" );

    while (credit > 0 && sent < count) {
        logger(trace) << "Sending messages through the link";
        
        m.id(sent);
        s.send(m);
        logger(trace) << "Sent message: " << m.body().as_string();
         
        sent++;
        timer.reset();
    }    
}

void SenderHandler::on_delivery_accept(event& e)
{
    logger(debug) << "Event name: " << e.name();
    logger(trace) << "Message accepted. Now obtaining the connection reference object";
    connection conn = e.connection();

    logger(trace) << "Confirmed message delivery";
    confirmedSent++;
    
    if (confirmedSent == count) { 
        logger(trace) << "Closing the sender after sending " << confirmedSent 
            << " message" << (confirmedSent > 1 ? "s" : "" );
        conn.close();
    }
}

void SenderHandler::on_transport_close(event &e) {
    logger(debug) << "Event name: " << e.name();
}


void SenderHandler::on_transport_error(event &e) {
    logger(error) << "The connection with " << broker_url.host_port() << 
            " was interrupted";
    logger(debug) << "Event name: " << e.name();
}

void SenderHandler::on_connection_close(event &e)
{
    logger(debug) << "Stopping reactor";
    logger(debug) << "Event name: " << e.name();
    timeoutTask = NULL;
}

void SenderHandler::on_connection_error(event &e)
{
    logger(error) << "Failed to connect to " << broker_url.host_port();
    logger(debug) << "Event name: " << e.name();
    
     if (!timeoutTask) {
        logger(debug) << "Quiescing, therefore ignoring event: " << e.name();

        return;
    }
    
    timeoutTask = NULL;
}


void SenderHandler::on_timer(event &e)
{
    if (!timeoutTask) {
        logger(debug) << "Quiescing, therefore ignoring event: " << e.name();

        return;
    }

    if (timer.isExpired()) {
        logger(info) << "Timed out";

        /**
         * TODO: this is, certainly, a bad way to exit. However, upstream does 
         * not yet have a stable interface for timers. This should be fixed in 
         * the future.
         */
        exit(1);
    } else {
        timer--;
        logger(debug) << "Waiting ...";
        e.container().schedule(1000);
    }
}

void SenderHandler::setCount(int count)
{
    this->count = count;
}

int SenderHandler::getCount() const
{
    return count;
}

void SenderHandler::setMessage(message &msg)
{
    this->m = msg;
}

message SenderHandler::getMessage() const
{
    return m;
}


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */