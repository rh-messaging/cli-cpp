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
    ConnectorHandler(const string &url);

    virtual ~ConnectorHandler();

    void on_start(event &e);
    void on_connection_opened(event &e);
    void on_connection_error(event &e);
    void on_connection_local_open(event& e);
    void on_connection_remote_open(event& e);
    
    void on_transport_error(event &e);
    void on_session_error(event &e);
    void on_link_error(event &e);

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

