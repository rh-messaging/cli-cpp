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
    m(),
    timer_event(*this)
{

}

SenderHandler::~SenderHandler()
{
    logger(debug) << "Destroying the sender handler";
}

void SenderHandler::timerEvent() {
    if (timer.isExpired()) {
        logger(info) << "Timed out";

        sndr.container().stop();
    } else {
        timer--;
        logger(debug) << "Waiting ...";
        
        duration d = duration(int(1000 * duration::SECOND.milliseconds()));
        sndr.container().schedule(d, timer_event);
    }
}

void SenderHandler::on_container_start(container &c)
{
    logger(debug) << "Starting messaging handler";
        
    logger(debug) << "Creating a sender";
    sndr = c.open_sender(broker_url);
    
    logger(trace) << "Setting up timer";
    duration d = duration(1000 * duration::SECOND.milliseconds());
    c.schedule(d, timer_event);
}

void SenderHandler::on_sendable(sender &s)
{
    logger(debug) << "Preparing to send message";
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


void SenderHandler::on_tracker_accept(tracker &t)
{
    
    logger(trace) << "Message accepted. Now obtaining the connection reference object";
    connection conn = t.connection();

    logger(trace) << "Confirmed message delivery";
    confirmedSent++;
    
    if (confirmedSent == count) { 
        logger(trace) << "Closing the sender after sending " << confirmedSent 
            << " message" << (confirmedSent > 1 ? "s" : "" );
        conn.close();
    }
}


void SenderHandler::on_transport_close(transport &t) {
    logger(debug) << "Closing the transport";
}


void SenderHandler::on_transport_error(transport &t) {
    logger(error) << "The connection with " << broker_url.host_port() << 
            " was interrupted";
}

void SenderHandler::on_connection_close(connection &c)
{
    logger(debug) << "Closing connection";
}

void SenderHandler::on_connection_error(connection &c)
{
    logger(error) << "Failed to connect to " << broker_url.host_port();
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