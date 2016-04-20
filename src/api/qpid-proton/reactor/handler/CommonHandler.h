/*
 * SimpleMessagingHandler.h
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#ifndef COMMON_HANDLER_H_
#define COMMON_HANDLER_H_

#include <proton/container.hpp>
#include <proton/handler.hpp>
#include <proton/connection.hpp>
#include <proton/event.hpp>

#include <proton/url.hpp>
#include <proton/task.hpp>

#include "Timer.h"

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

using proton::handler;
using proton::url;
using proton::task;
using proton::event;

namespace dtests {
namespace proton {
namespace reactor {

using std::string;
using dtests::common::Timer;


/**
 * An abstract proton message handler providing a common interface for other
 * client handlers
 */
class CommonHandler : public handler {
  public:
    /**
     * Constructor
     * @param url broker URL
     */
    CommonHandler(const string &url, int timeout = 10);
    virtual ~CommonHandler();
    
  protected:    
    /**
     * Logger
     */
    static Logger logger;
    
    /**
     * Broker URL
     */
    url broker_url;
    
    task *timeoutTask;
    Timer timer;
    
    void setupTimer(event &e);
    void timerEvent(event &e);
    void disableTimer();

  private:
    typedef handler super;

    


};



} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* COMMON_HANDLER_H_ */
