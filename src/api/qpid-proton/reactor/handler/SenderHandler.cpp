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

SenderHandler::SenderHandler(const string &url)
    : super(url),
    count(1),
    sent(0),
    confirmedSent(0)

{


}

SenderHandler::~SenderHandler()
{
    logger(debug) << "Destroying the sender handler";
}

void SenderHandler::on_start(event &e)
{
    logger(debug) << "Starting messaging handler";

    logger(debug) << "Creating a sender";
    e.container().open_sender(broker_url);
}

void SenderHandler::on_sendable(event &e)
{
    logger(debug) << "Event name: " << e.name();
    int credit = e.sender().credit();

    if (credit == 0) {
        logger(warning) << "There not enough credit to send messages";

    }

    logger(debug) << "The handler has enough credit to send " << credit
            << " messages";
    logger(debug) << "The handler has sent " << sent << " messages";


    while (credit > 0 && sent < count) {
        logger(trace) << "Creating the message object and setting default values";

        logger(trace) << "Sending messages through the link";
        e.sender().send(this->m);

        logger(trace) << "Closing the sender";
        e.sender().close();
        sent++;
    }
}

void SenderHandler::on_accepted(event& e)
{
    logger(debug) << "Event name: " << e.name();
    logger(trace) << "Message accepted. Now obtaining the connection reference object";

    do {
        confirmedSent++;
        connection &conn = e.connection();

        logger(debug) << "Closing connection";
        conn.close();
        logger(debug) << "Connection closed";
    } while (confirmedSent < count);
}

void SenderHandler::on_disconnected(event& e)
{
    logger(debug) << "Event name: " << e.name();
}

void SenderHandler::on_connection_closed(event &e)
{
    logger(debug) << "Stopping reactor";
    logger(debug) << "Event name: " << e.name();
}

void SenderHandler::on_connection_error(event &e)
{
    logger(error) << "Failed to connect to " << broker_url.host_port();
    logger(debug) << "Event name: " << e.name();
}

void SenderHandler::setCount(int count)
{
    this->count = count;
}

int SenderHandler::getCount() const
{
    return count;
}

void SenderHandler::setMessage(message m)
{
    this->m = m;
}

message SenderHandler::getMessage() const
{
    return m;
}


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */