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
#include <proton/transport.hpp>
#include <proton/error_condition.hpp>
#include <proton/message_id.hpp>
#include <proton/source_options.hpp>
#include <proton/connection_options.hpp>
#include <proton/sender_options.hpp>
#include <proton/thread_safe.hpp>

#include "CommonHandler.h"
#include "Timer.h"
#include "Utils.h"

using proton::message;
using proton::message_id;
using proton::connection;
using proton::sender;
using proton::delivery;
using proton::source_options;
using proton::transport;
using proton::tracker;
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
     * @param conn_urls connection URLs
     * @param is_topic if target is topic
     * @param user username
     * @param password password
     * @param sasl_mechanisms SASL mechanisms
     * @param conn_sasl_enabled enable connection SASL
     * @param conn_ssl_certificate path to client certificate
     * @param conn_ssl_private_key path to client private key
     * @param conn_ssl_password client's certificate database password
     * @param conn_ssl_trust_store path to client trust store
     * @param conn_ssl_verify_peer verifies server certificate
     * @param conn_ssl_verify_peer_name verifies connection url against server hostname
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
     * @param conn_use_config_file use configuration file for connection
     * @param log_msgs message log format
     */
    SenderHandler(
        const string &url,
        vector<string> conn_urls,
        bool is_topic,
        string user,
        string password,
        string sasl_mechanisms,
        string conn_sasl_enabled = "true",
        string conn_ssl_certificate = "",
        string conn_ssl_private_key = "",
        string conn_ssl_password = "",
        string conn_ssl_trust_store = "",
        bool conn_ssl_verify_peer = false,
        bool conn_ssl_verify_peer_name = false,
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
        bool conn_use_config_file = false,
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
    
    void on_transport_error(transport &t);
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

    void send();
    void checkIfCanSend();
    
    /**
     * Gets the message to send
     * @return the message to send
     */
    message getMessage() const;

  private:
    typedef CommonHandler super;
    bool ready;
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

    duration interval;

};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* SENDERHANDLER_H */

