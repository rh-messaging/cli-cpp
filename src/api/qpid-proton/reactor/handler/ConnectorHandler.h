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
     */
    ConnectorHandler(const string &url, int timeout = 10);

    virtual ~ConnectorHandler();

    void on_container_start(event &e, container &c);
    void on_connection_open(event &e, connection &conn);
    void on_connection_close(event &e, connection &conn);
    void on_connection_error(event &e, connection &conn);
   
    void on_transport_error(event &e, transport &trans);
    
    void on_session_error(event &e, session &s);
    
#ifdef REACTIVE_HAS_TIMER_
    void on_timer(event &e);
#endif // REACTIVE_HAS_TIMER_

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
       
    void closeObjects();
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* CONNECTORHANDLER_H */

