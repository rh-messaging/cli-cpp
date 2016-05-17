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
using proton::container;
using proton::connection;
using proton::receiver;

using proton::endpoint;

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
     */
    ReceiverHandler(const string &url, int timeout = 10);

    virtual ~ReceiverHandler();

    void on_container_start(event &e, container &c);
    void on_message(event &e, message &m);
    void on_delivery_accept(event &e);
    void on_delivery_reject(event &e);
    void on_connection_close(event &e);
    void on_timer(event &e, container &c);

  private:
    typedef CommonHandler super;
     
    void do_disconnect();
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* RECEIVERHANDLER_H */

