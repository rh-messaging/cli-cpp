/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SenderHandler.h
 * Author: opiske
 *
 * Created on October 16, 2015, 10:19 AM
 */

#ifndef SENDERHANDLER_H
#define SENDERHANDLER_H

#include <proton/tracker.hpp>
#include <proton/message_id.hpp>
#include <proton/connection_options.hpp>
#include <proton/reconnect_timer.hpp>
#include <proton/thread_safe.hpp>

#include "CommonHandler.h"
#include "Timer.h"
#include "Utils.h"

using proton::message;
using proton::message_id;
using proton::connection;
using proton::sender;
using proton::delivery;
using proton::transport;
using proton::tracker;
using proton::reconnect_timer;
using proton::connection_options;

namespace dtests {
namespace proton {
namespace reactor {

using dtests::common::Timer;

/**
 * A proton message handler that handles message send events
 */
class SenderHandler : public CommonHandler {
  public:
    /**
     * Constructor
     * @param url broker URL
     * @param user username
     * @param password password
     * @param sasl_mechanisms SASL mechanisms
     * @param timeout timeout
     * @param duration message actions total duration
     * @param duration_mode specifies where to wait to achieve expected duration
     * @param conn_reconnect type or reconnection
     * @param conn_reconnect_interval reconnect interval
     * @param conn_reconnect_limit reconnect limit
     * @param conn_reconnect_timeout reconnect timeout
     * @param conn_reconnect_first reconnect first
     * @param conn_reconnect_increment reconnect increment
     * @param conn_reconnect_doubling reconnect doubling
     * @param conn_reconnect_custom custom reconnect values
     * @param conn_heartbeat connection heartbeat in seconds
     * @param max_frame_size maximum frame size
     * @param log_msgs message log format
     */
    SenderHandler(
        const string &url,
        string user,
        string password,
        string sasl_mechanisms,
        int timeout = 10,
        int duration_time = 0,
        string duration_mode = "after-send",
        string conn_reconnect = "true",
        int32_t conn_reconnect_interval = -1,
        int32_t conn_reconnect_limit = -1,
        int32_t conn_reconnect_timeout = -1,
        uint32_t conn_reconnect_first = 0,
        uint32_t conn_reconnect_increment = 100,
        bool conn_reconnect_doubling = true,
        bool conn_reconnect_custom = false,
        uint32_t conn_heartbeat = 0,
        uint32_t max_frame_size = -1,
        string log_msgs = ""
    );
    
    void timerEvent();

    virtual ~SenderHandler();

    void on_container_start(container &c);
    void on_sendable(sender &s);
    void on_connection_error(connection &c);
    void on_connection_close(connection &c);
    void on_tracker_accept(tracker &t);
    void on_tracker_reject(tracker &t);
    
    // void on_transport_error(transport &t);
    void on_transport_close(transport &t);

    /**
     * Sets the message count
     * @param count the message count
     */
    void setCount(int count);
    
    /**
     * Gets the message count
     * @return the message count
     */
    int getCount() const;
    
    /**
     * Sets the message to send
     * @param m the message to send
     */
    void setMessage(message &m);
    
    /**
     * Gets the message to send
     * @return the message to send
     */
    message getMessage() const;

  private:
    typedef CommonHandler super;
    int count;
    int duration_time;
    string duration_mode;
    int sent;
    int confirmedSent;
    sender sndr;

    message m;
    
    struct timer_event_t : public void_function0 {
        SenderHandler &parent;
        timer_event_t(SenderHandler &handler) : parent(handler) {}
        void operator()() { 
            parent.timerEvent();
        }
    };
    
    timer_event_t timer_event;

};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* SENDERHANDLER_H */

