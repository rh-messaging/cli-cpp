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

#include <vector>

#include <proton/container.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/connection.hpp>

#include <proton/function.hpp>

#include <proton/message.hpp>
#include <proton/work_queue.hpp>


#include "Timer.h"
#include "UriParser.h"

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

using proton::messaging_handler;
using proton::container;
using proton::void_function0;
using proton::duration;
using proton::work_queue;

namespace dtests {
namespace proton {
namespace reactor {

using std::string;
using std::vector;
using dtests::common::Timer;
using dtests::common::UriParser;


/**
 * An abstract proton message handler providing a common interface for other
 * client handlers
 */
class CommonHandler : public messaging_handler {
  public:
    /**
     * Constructor
     * @param url broker URL
     * @param conn_urls connection URLs
     * @param is_topic if target is topic
     * @param user username
     * @param password password
     * @param sasl_mechanisms SASL mechanisms
     * @param conn_sasl_enabled enable connection SASL
     * @param conn_ssl_certificate path to client certificate
     * @param conn_ssl_private_key path to client private key
     * @param conn_ssl_password client's certificate database password
     * @param conn_ssl_trust_store path to client trust store
     * @param conn_ssl_verify_peer verifies server certificate
     * @param conn_ssl_verify_peer_name verifies connection url against server hostname
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
     * @param conn_use_config_file use configuration file for connection
     * @param log_msgs message log format
     */
    CommonHandler(
        const string &url,
        vector<string> conn_urls,
        bool is_topic,
        string user,
        string password,
        string sasl_mechanisms,
        string conn_sasl_enabled = "true",
        string conn_ssl_certificate = "",
        string conn_ssl_private_key = "",
        string conn_ssl_password = "",
        string conn_ssl_trust_store = "",
        bool conn_ssl_verify_peer = false,
        bool conn_ssl_verify_peer_name = false,
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
        bool conn_use_config_file = false,
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
     * Work queue
     */
    work_queue* work_q;
    
    /**
     * Broker URL
     */
    UriParser broker_url;

    /**
     * Connection URLs
     */
    vector<string> conn_urls;

    /**
     * If is topic
     */
    bool is_topic;

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
     * Connection SASL enabled
     */
    string conn_sasl_enabled;
    
    /**
     * SLL certificate
     */
    string conn_ssl_certificate;

    /**
     * SLL private key
     */
    string conn_ssl_private_key;

    /**
     * SLL password
     */
    string conn_ssl_password;

    /**
     * SLL trust store
     */
    string conn_ssl_trust_store;

    /**
     * SLL verify peer
     */
    bool conn_ssl_verify_peer;

    /**
     * SLL verify peer name
     */
    bool conn_ssl_verify_peer_name;

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
     * Use of connection config file
     */
    bool conn_use_config_file;

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
