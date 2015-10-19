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
  private:
    typedef CommonHandler super;

};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* RECEIVERHANDLER_H */

