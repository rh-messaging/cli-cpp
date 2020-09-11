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
#include <proton/connection.hpp>

using proton::connection;
using proton::session;
using proton::sender;
using proton::receiver;
using proton::event;
using proton::container;
using proton::transport;

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
     * @param user username
     * @param password password
     * @param sasl_mechanisms SASL mechanisms
     */
    ConnectorHandler(const string &url, string user, string password, string sasl_mechanisms, int timeout = 10);

    virtual ~ConnectorHandler();
    
    void timerEvent();

    void on_container_start(event &e, container &c);
    void on_connection_open(event &e, connection &conn);
    void on_connection_close(event &e, connection &conn);
    void on_connection_error(event &e, connection &conn);
   
    void on_transport_error(event &e, transport &trans);
    
    void on_session_error(event &e, session &s);
    

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
    connection conn;
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

