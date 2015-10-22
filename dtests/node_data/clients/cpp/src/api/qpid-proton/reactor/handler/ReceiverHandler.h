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

#include "CommonHandler.h"
#include "reactor/formatter/ReactorDecoder.h"
#include "formatter/DictFormatter.h"
 #include "formatter/DictWriter.h"

 #include "formatter/UpstreamFormatter.h"
 #include "formatter/UpstreamWriter.h"

using proton::message;
using proton::event;
using proton::connection;
using proton::sender;
using proton::counted_ptr;

namespace dtests {
namespace proton {
namespace reactor {

class ReceiverHandler: public CommonHandler {
  public:
    ReceiverHandler(const string &url);
    
    virtual ~ReceiverHandler();
    
    void on_start(event &e);
    void on_message(event& e);
    void on_accepted(event& e);
    void on_disconnected(event& e);
    
  private:
    typedef CommonHandler super;

};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* RECEIVERHANDLER_H */

