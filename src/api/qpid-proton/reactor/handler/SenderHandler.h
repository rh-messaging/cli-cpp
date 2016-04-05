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

#include "CommonHandler.h"

using proton::message;
using proton::event;
using proton::connection;
using proton::sender;

namespace dtests {
namespace proton {
namespace reactor {

/**
 * A proton message handler that handles message send events
 */
class SenderHandler : public CommonHandler {
  public:
    /**
     * Constructor
     * @param url broker URL
     */
    SenderHandler(const string &url);

    virtual ~SenderHandler();

    void on_start(event &e);
    void on_sendable(event &e);
    void on_connection_error(event &e);
    void on_connection_closed(event &e);
    void on_delivery_accept(event& e);
    void on_disconnected(event& e);

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
    void setMessage(message m);
    
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

    message m;

};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* SENDERHANDLER_H */

