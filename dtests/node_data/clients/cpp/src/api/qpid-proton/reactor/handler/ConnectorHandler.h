/* 
 * File:   ConnectorHandler.h
 * Author: opiske
 *
 * Created on October 14, 2015, 3:33 PM
 */

#ifndef CONNECTORHANDLER_H
#define CONNECTORHANDLER_H

#include "CommonHandler.h"

#include <proton/messaging_handler.hpp>

namespace dtests {
namespace proton {
namespace reactor {

class ConnectorHandler: public CommonHandler {
public:
    ConnectorHandler(const string &url);
    
    virtual ~ConnectorHandler();
    
    void on_start(event &e);
    void on_connection_opened(event& e);

private:

    typedef CommonHandler super;
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* CONNECTORHANDLER_H */

