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

#include "CommonHandler.h"

#include "reactor/formatter/ReactorDecoder.h"
#include "formatter/DictFormatter.h"
#include "formatter/DictWriter.h"

#include "formatter/UpstreamFormatter.h"
#include "formatter/UpstreamWriter.h"

using proton::message;
using proton::container;
using proton::connection;
using proton::receiver;
using proton::delivery;
using proton::tracker;
using proton::void_function0;
using proton::endpoint;

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
     */
    ReceiverHandler(const string &url, string msg_action, int msg_action_size, string user, string password, string sasl_mechanisms, int timeout = 10);
    
    void timerEvent();

    virtual ~ReceiverHandler();

    void on_container_start(container &c);
    void do_message_action(delivery &d);
    void on_message(delivery &d, message &m);
    void on_tracker_accept(tracker &t);
    void on_tracker_reject(tracker &t);
    void on_connection_close(connection &conn);

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
     
    void do_disconnect();
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* RECEIVERHANDLER_H */

