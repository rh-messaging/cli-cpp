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

    void on_container_start(container &c);
    void on_message(delivery &d, message &m);
    void on_tracker_accept(tracker &t);
    void on_tracker_reject(tracker &t);
    void on_connection_close(connection &conn);
    
#ifdef REACTIVE_HAS_TIMER_
    void on_timer(event &e, container &c);
#endif // REACTIVE_HAS_TIMER_

  private:
    typedef CommonHandler super;
     
    void do_disconnect();
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* RECEIVERHANDLER_H */

