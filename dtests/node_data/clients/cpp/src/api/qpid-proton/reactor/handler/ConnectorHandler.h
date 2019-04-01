/* 
 * File:   ConnectorHandler.h
 * Author: opiske
 *
 * Created on October 14, 2015, 3:33 PM
 */

#ifndef CONNECTORHANDLER_H
#define CONNECTORHANDLER_H


#include "CommonHandler.h"
#include "ObjectControl.h"

#include <list>
#include <proton/transport.hpp>
#include <proton/error_condition.hpp>
#include <proton/connection.hpp>
#include <proton/connection_options.hpp>
#include <proton/thread_safe.hpp>

using proton::connection;
using proton::session;
using proton::sender;
using proton::receiver;
using proton::event;
using proton::container;
using proton::transport;
using proton::connection_options;

namespace dtests {
namespace proton {
namespace reactor {

using std::list;

/**
 * A proton message handler that handles message connection-specific events
 */
class ConnectorHandler : public CommonHandler {
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
     * @param timeout timeout
     * @param conn_reconnect type of reconnection
     * @param conn_reconnect_interval reconnect interval
     * @param conn_reconnect_limit reconnect limit
     * @param conn_reconnect_timeout reconnect timeout
     * @param conn_reconnect_first reconnect first
     * @param conn_reconnect_increment reconnect increment
     * @param conn_reconnect_doubling reconnect doubling
     * @param conn_reconnect_custom custom reconnect values
     * @param max_frame_size maximum frame size
     * @param conn_use_config_file use configuration file for connection
     */
    ConnectorHandler(
        const string &url,
        vector<string> conn_urls,
        bool is_topic,
        string user,
        string password,
        string sasl_mechanisms,
        string conn_sasl_enabled = "true",
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
        bool conn_use_config_file = false
    );

    void timerEvent();

    void on_container_start(container &c);
    void on_connection_open(connection &conn);
    void on_connection_close(connection &conn);
    void on_connection_error(connection &conn);
   
    void on_transport_error(transport &trans);
    void on_transport_close(transport &t);
    
    void on_session_error(session &s);
    

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
     * Sets the object control (CESR)
     */
    void setObjectControl(int control);

  private:

    typedef CommonHandler super;
    int count;
    
    int objectControl;
    connection connector_conn;
    session sessionObj; 
    sender senderObj;
    receiver receiverObj;
    
    struct timer_event_t : public void_function0 {
        ConnectorHandler &parent;
        timer_event_t(ConnectorHandler &handler) : parent(handler) {}
        void operator()() { 
            parent.timerEvent();
        }
    };
    
    timer_event_t timer_event;
       
    void closeObjects();
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* CONNECTORHANDLER_H */

