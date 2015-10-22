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
using proton::counted_ptr;

namespace dtests {
namespace proton {
namespace reactor {

class SenderHandler : public CommonHandler {
  public:
    SenderHandler(const string &url);

    virtual ~SenderHandler();

    void on_start(event &e);
    void on_sendable(event &e);
    void on_connection_error(event &e);
    void on_connection_closed(event &e);
    void on_accepted(event& e);
    void on_disconnected(event& e);


    void setCount(int count);
    int getCount() const;
    void setMessage(message m);
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

