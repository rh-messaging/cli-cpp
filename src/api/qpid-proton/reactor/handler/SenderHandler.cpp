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

SenderHandler::SenderHandler(
    const string &url,
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
    uint32_t max_frame_size
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
        max_frame_size
    ),
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
#if defined(__REACTOR_HAS_TIMER)
    if (timer.isExpired()) {
        logger(info) << "Timed out";

        sndr.container().stop();
    } else {
        timer--;
        logger(debug) << "Waiting ...";
        
        duration d = duration(int(1000 * duration::SECOND.milliseconds()));
        sndr.container().schedule(d, timer_event);
    }
#endif
}

void SenderHandler::on_container_start(container &c)
{
    logger(debug) << "Starting messaging handler";

    logger(debug) << "User: " << user;
    logger(debug) << "Password: " << password;
    logger(debug) << "SASL mechanisms: " << sasl_mechanisms;

    logger(debug) << "Maximum frame size: " << max_frame_size;

    connection_options conn_opts = c.client_connection_options()
                                    .user(user)
                                    .password(password)
                                    .sasl_enabled(true)
                                    .sasl_allow_insecure_mechs(true)
                                    .sasl_allowed_mechs(sasl_mechanisms)
                                    .max_frame_size(max_frame_size);

    logger(debug) << "Setting a reconnect timer: " << conn_reconnect;
    logger(debug) << "Custom reconnect: " << conn_reconnect_custom;
    
    if (conn_reconnect == "true" && conn_reconnect_custom == false) {
        conn_opts = conn_opts.reconnect(reconnect_timer());
    } else if (conn_reconnect == "true" && conn_reconnect_custom == true) {
        logger(debug) << "Reconnect first: " << conn_reconnect_first;
        logger(debug) << "Reconnect interval (max): " << conn_reconnect_interval;
        logger(debug) << "Reconnect increment: " << conn_reconnect_increment;
        logger(debug) << "Reconnect doubling: " << conn_reconnect_doubling;
        logger(debug) << "Reconnect limit (max_retries): " << conn_reconnect_limit;
        logger(debug) << "Reconnect timeout: " << conn_reconnect_timeout;

        conn_opts = conn_opts.reconnect(
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

    logger(debug) << "Creating a sender";
    sndr = c.open_sender(
            broker_url,
            conn_opts
    );
    
    logger(trace) << "Setting up timer";
    duration d = duration(1000 * duration::SECOND.milliseconds());
#if defined(__REACTOR_HAS_TIMER)
    c.schedule(d, timer_event);
#endif
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
        
        m.id(message_id(sent));
        s.send(m);
        logger(trace) << "Sent message: " << m.body().as_string();
         
        sent++;
#if defined(__REACTOR_HAS_TIMER)
        timer.reset();
#endif
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

/*
void SenderHandler::on_transport_error(transport &t) {
    logger(error) << "The connection with " << broker_url.host_port() << 
            " was interrupted";
}
*/

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
