/*
 * SimpleMessagingHandler.h
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#ifndef COMMON_HANDLER_H_
#define COMMON_HANDLER_H_

#include <proton/container.hpp>
#include <proton/default_container.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/connection.hpp>

#include <proton/url.hpp>
#include <proton/function.hpp>



#include "Timer.h"

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

using proton::messaging_handler;
using proton::url;
using proton::task;
using proton::container;
using proton::void_function0;
using proton::duration;

namespace dtests {
namespace proton {
namespace reactor {

using std::string;
using dtests::common::Timer;


/**
 * An abstract proton message handler providing a common interface for other
 * client handlers
 */
class CommonHandler : public messaging_handler {
  public:
    /**
     * Constructor
     * @param url broker URL
     */
    CommonHandler(const string &url, int timeout = 10);
    virtual ~CommonHandler();
    
    virtual void timerEvent() = 0;
        
  protected:    
    
      
    /**
     * Logger
     */
    static Logger logger;
    
    /**
     * Broker URL
     */
    url broker_url;
    
    container *cont;
    
    Timer timer;

  private:
    
    typedef messaging_handler super;

    


};



} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* COMMON_HANDLER_H_ */
