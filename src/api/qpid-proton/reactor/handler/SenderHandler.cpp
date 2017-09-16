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

#include "reactor/formatter/ReactorDecoder.h"
#include "formatter/DictFormatter.h"
#include "formatter/DictWriter.h"

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
    int duration_time,
    string duration_mode,
    string conn_reconnect,
    int32_t conn_reconnect_interval,
    int32_t conn_reconnect_limit,
    int32_t conn_reconnect_timeout,
    uint32_t conn_reconnect_first,
    uint32_t conn_reconnect_increment,
    bool conn_reconnect_doubling,
    bool conn_reconnect_custom,
    uint32_t conn_heartbeat,
    uint32_t max_frame_size,
    string log_msgs
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
        conn_heartbeat,
        max_frame_size,
        log_msgs
    ),
    count(1),
    duration_time(duration_time),
    duration_mode(duration_mode),
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

        duration d = duration(1 * duration::SECOND.milliseconds());
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

    connection_options conn_opts;

    if (!user.empty()) conn_opts.user(user);
    if (!password.empty()) conn_opts.password(password);

    conn_opts.sasl_enabled(true);
    conn_opts.sasl_allow_insecure_mechs(true);
    conn_opts.sasl_allowed_mechs(sasl_mechanisms);
    // conn_opts.max_frame_size(max_frame_size);

    logger(debug) << "Setting a reconnect timer: " << conn_reconnect;
    logger(debug) << "Custom reconnect: " << conn_reconnect_custom;
    
    configure_reconnect(conn_opts);

    if (conn_heartbeat != 0) {
        logger(debug) << "Heartbeat: " << conn_heartbeat;

        duration heartbeat_seconds = conn_heartbeat * duration::SECOND;

        conn_opts.idle_timeout(heartbeat_seconds);
    }

    logger(debug) << "Creating a sender";
    sndr = c.open_sender(
            broker_url,
            conn_opts
    );
    
    logger(trace) << "Setting up timer";
    duration d = duration(this->timeout * duration::SECOND.milliseconds());
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

    double ts = get_time();

    while (credit > 0 && sent < count) {
        logger(trace) << "Sending messages through the link";
        
        if (duration_time > 0 && duration_mode == "before-send") {
            sleep4next(ts, count, duration_time, sent+1);
        }

        s.send(m);
        
        // logger(trace) << "Sent message: " << m.body().as_string();

        if (log_msgs == "dict") {
            ReactorDecoder decoder = ReactorDecoder(m);

            std::ostringstream stream;
            DictWriter writer = DictWriter(&stream);

            DictFormatter formatter = DictFormatter();
            formatter.printMessage(&decoder, &writer);

            writer.endLine();
            std::cout << writer.toString();
        } else if (log_msgs == "interop") {
            DictFormatter formatter = DictFormatter();

            formatter.printMessageInterop(m);
        }

        if (duration_time > 0 && duration_mode == "after-send") {
            sleep4next(ts, count, duration_time, sent+1);
        }

        if (duration_time > 0 && duration_mode == "after-send-tx-action") {
            // TODO: Transactions are not supported yet
        }
         
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

    if (conn_reconnect == "false") {
        exit(1);
    }
}

void SenderHandler::on_transport_error(transport &t) {
    logger(error) << "The connection with " << broker_url.host_port() << " was interrupted: " << t.error().what();

    if (t.error().what().find("unauthorized") != string::npos) {
        exit(1);
    }
}

void SenderHandler::on_tracker_reject(tracker &t)
{
    std::cerr << "[error] Delivery rejected" << std::endl;

    exit(1);
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
