/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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
private:

    typedef CommonHandler super;
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* CONNECTORHANDLER_H */

