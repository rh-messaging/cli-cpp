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

#include <proton/task.hpp>

#include "CommonHandler.h"
#include "Timer.h"

using proton::message;
using proton::event;
using proton::connection;
using proton::sender;
using proton::task;
using proton::delivery;
using proton::transport;

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
     */
    SenderHandler(const string &url, int timeout = -1);

    virtual ~SenderHandler();

    void on_container_start(event &e, container &c);
    void on_sendable(event &e, sender &s);
    void on_connection_error(event &e, connection &c);
    void on_connection_close(event &e, connection &c);
    void on_delivery_accept(event &e, delivery &d);
    
    void on_transport_error(event &e, transport &t);
    void on_transport_close(event &e, transport &t);
    
#ifdef REACTIVE_HAS_TIMER_
    void on_timer(event &e);
#endif // REACTIVE_HAS_TIMER_

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
    int sent;
    int confirmedSent;

#ifdef REACTIVE_HAS_TIMER_    
    task *timeoutTask;
    Timer timer;
#endif // REACTIVE_HAS_TIMER_
    message m;

};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* SENDERHANDLER_H */

