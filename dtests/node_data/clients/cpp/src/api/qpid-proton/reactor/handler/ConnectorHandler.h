/* 
 * File:   ConnectorHandler.h
 * Author: opiske
 *
 * Created on October 14, 2015, 3:33 PM
 */

#ifndef CONNECTORHANDLER_H
#define CONNECTORHANDLER_H

#include <list>
#include <proton/messaging_handler.hpp>
#include <proton/connection.hpp>

#include "CommonHandler.h"

using proton::connection;
using proton::event;

namespace dtests {
namespace proton {
namespace reactor {

using std::list;

class ConnectorHandler : public CommonHandler {
  public:
    ConnectorHandler(const string &url);

    virtual ~ConnectorHandler();

    void on_start(event &e);
    void on_connection_opened(event &e);
    void on_connection_error(event &e);
    void on_connection_local_open(event& e);
    void on_connection_remote_open(event& e);


    
    void setCount(int count);
    int getCount() const;



  private:

    typedef CommonHandler super;
    int count;

};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* CONNECTORHANDLER_H */

