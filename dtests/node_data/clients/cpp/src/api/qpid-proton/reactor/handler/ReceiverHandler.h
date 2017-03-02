/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReceiverHandler.h
 * Author: opiske
 *
 * Created on October 19, 2015, 4:43 PM
 */

#ifndef RECEIVERHANDLER_H
#define RECEIVERHANDLER_H

#include <proton/delivery.hpp>
#include <proton/tracker.hpp>
#include <proton/source_options.hpp>
#include <proton/connection_options.hpp>
#include <proton/receiver_options.hpp>
#include <proton/reconnect_timer.hpp>
#include <proton/thread_safe.hpp>

#include "CommonHandler.h"

#include "reactor/formatter/ReactorDecoder.h"
#include "formatter/DictFormatter.h"
#include "formatter/DictWriter.h"

#include "formatter/UpstreamFormatter.h"
#include "formatter/UpstreamWriter.h"

using proton::message;
using proton::container;
using proton::connection;
using proton::sender;
using proton::receiver;
using proton::delivery;
using proton::tracker;
using proton::void_function0;
using proton::endpoint;
using proton::source;
using proton::source_options;
using proton::reconnect_timer;
using proton::transport;
using proton::connection_options;

#ifdef PN_CPP_HAS_STD_FUNCTION
#undef PN_CPP_HAS_STD_FUNCTION
#endif 

namespace dtests {
namespace proton {
namespace reactor {

/**
 * A proton message handler that handles message receive events
 */
class ReceiverHandler : public CommonHandler {
  public:
    /**
     * Constructor
     * @param url broker URL
     * @param msg_action message action
     * @param msg_action_size apply action in the batch of given size
     * @param user username
     * @param password password
     * @param sasl_mechanisms SASL mechanisms
     * @param timeout timeout
     * @param count count of messages to receive
     * @param conn_reconnect type of reconnection
     * @param conn_reconnect_interval reconnect interval
     * @param conn_reconnect_limit reconnect limit
     * @param conn_reconnect_timeout reconnect timeout
     * @param conn_reconnect_first reconnect first
     * @param conn_reconnect_increment reconnect increment
     * @param conn_reconnect_doubling reconnect doubling
     * @param conn_reconnect_custom custom reconnect values
     * @param max_frame_size maximum frame size
     * @param log_msgs message log format
     * @param process_reply_to send message to reply-to address if enabled and message got reply-to address
     * @param browse enable browsing receiver
     */
    ReceiverHandler(
        const string &url,
        string msg_action,
        int msg_action_size,
        string user,
        string password,
        string sasl_mechanisms,
        int timeout = 10,
        int count = 0,
        string conn_reconnect = "true",
        int32_t conn_reconnect_interval = -1,
        int32_t conn_reconnect_limit = -1,
        int32_t conn_reconnect_timeout = -1,
        uint32_t conn_reconnect_first = 0,
        uint32_t conn_reconnect_increment = 100,
        bool conn_reconnect_doubling = true,
        bool conn_reconnect_custom = false,
        uint32_t max_frame_size = -1,
        string log_msgs = "",
        bool process_reply_to = false,
        bool browse = false
    );
    
    void timerEvent();

    virtual ~ReceiverHandler();

    void on_container_start(container &c);
    void do_message_action(delivery &d);
    void do_process_reply_to(message &m);
    void on_message(delivery &d, message &m);
    void on_tracker_accept(tracker &t);
    void on_tracker_reject(tracker &t);
    void on_connection_close(connection &conn);
    void on_connection_error(connection &conn);

    //void on_transport_error(transport &t);
    void on_transport_close(transport &t);

  private:
    typedef CommonHandler super;
    receiver recv;
    
    struct timer_event_t : public void_function0 {
        ReceiverHandler &parent;
        timer_event_t(ReceiverHandler &handler): parent(handler) { }
        void operator()() { 
            parent.timerEvent();
        }
    };
    
    
    duration interval;
    timer_event_t timer_event;

    string msg_action;
    int msg_action_size;
    int msg_received_cnt;
    bool process_reply_to;
    bool browse;
    int count;
    std::map<string, sender> senders;

    void do_disconnect();
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* RECEIVERHANDLER_H */

