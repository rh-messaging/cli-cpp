/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConnectorHandler.cpp
 * Author: opiske
 * 
 * Created on October 14, 2015, 3:33 PM
 */

#include "ConnectorHandler.h"

namespace dtests {
namespace proton {
namespace reactor {
	
using namespace dtests::common;
using namespace dtests::common::log;

ConnectorHandler::ConnectorHandler(const string &url)
	: super(url)
{
}

ConnectorHandler::~ConnectorHandler()
{
}

void ConnectorHandler::on_start(event &e) {
	logger(debug) << "Starting messaging handler";
	connection &conn = e.container().connect(broker_url);
	logger(debug) << "Connected to " << broker_url.path();
	
	logger(debug) << "Closing connection";
	conn.close();
	logger(debug) << "Connection closed";
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
