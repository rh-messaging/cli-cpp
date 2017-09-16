/*
 * CommonHandler.h
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#ifndef __REACTOR_HAS_TIMER
#define __REACTOR_HAS_TIMER
#endif

#ifndef COMMON_HANDLER_H_
#define COMMON_HANDLER_H_

#include <proton/container.hpp>
#include <proton/default_container.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/connection.hpp>

#include <proton/url.hpp>
#include <proton/function.hpp>

#include <proton/message.hpp>


#include "Timer.h"

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

using proton::messaging_handler;
using proton::url;
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
     * @param user username
     * @param password password
     * @param sasl_mechanisms SASL mechanisms
     * @param timeout timeout
     * @param conn_reconnect type of reconnection
     * @param conn_reconnect_interval reconnect interval
     * @param conn_reconnect_limit reconnect limit
     * @param conn_reconnect_timeout reconnect timeout
     * @param conn_reconnect_first reconnect first
     * @param conn_reconnect_increment reconnect increment
     * @param conn_reconnect_doubling reconnect doubling
     * @param conn_reconnect_custom custom reconnect values
     * @param conn_heartbeat connection heartbeat in seconds
     * @param max_frame_size maximum frame size
     * @param log_msgs message log format
     */
    CommonHandler(
        const string &url,
        string user,
        string password,
        string sasl_mechanisms,
        int timeout = 10,
        string conn_reconnect = "true",
        int32_t conn_reconnect_interval = -1,
        int32_t conn_reconnect_limit = -1,
        int32_t conn_reconnect_timeout = -1,
        uint32_t conn_reconnect_first = 0,
        uint32_t conn_reconnect_increment = 100,
        bool conn_reconnect_doubling = true,
        bool conn_reconnect_custom = false,
        uint32_t conn_heartbeat = 0,
        uint32_t max_frame_size = -1,
        string log_msgs = ""
    );
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

    /**
     * Username
     */
    string user;

    /**
     * Password
     */
    string password;

    /**
     * SASL mechanisms
     */
    string sasl_mechanisms;
    
    /**
     * Type of reconnection
     */
    string conn_reconnect;

    /**
     * Reconnect interval
     */
    int32_t conn_reconnect_interval;

    /**
     * Reconnect limit
     */
    int32_t conn_reconnect_limit;

    /**
     * Reconnect timeout
     */
    int32_t conn_reconnect_timeout;

    /**
     * Reconnect first
     */
    uint32_t conn_reconnect_first;

    /**
     * Reconnect increment
     */
    uint32_t conn_reconnect_increment;

    /**
     * Reconnect doubling
     */
    bool conn_reconnect_doubling;

    /**
     * Custom reconnect values
     */
    bool conn_reconnect_custom;

    /**
     * Connection heartbeat
     */
    uint32_t conn_heartbeat;

    /**
     * Maximum frame size
     */
    uint32_t max_frame_size;

    /**
     * Message log format
     */
    string log_msgs;

    container *cont;
    
    Timer timer;

    int timeout;
    void configure_reconnect(::proton::connection_options & conn_opts);

  private:
    
    typedef messaging_handler super;

    


};



} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* COMMON_HANDLER_H_ */
