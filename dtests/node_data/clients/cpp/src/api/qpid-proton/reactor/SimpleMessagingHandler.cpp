/*
 * SimpleMessagingHandler.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#include "SimpleMessagingHandler.h"

using proton::messaging_handler;
using proton::message;
using proton::event;
using proton::url;

namespace dtests {
namespace proton {
namespace reactor {

    
using namespace dtests::common;
using namespace dtests::common::log;

Logger SimpleMessagingHandler::logger = LoggerWrapper::getLogger();

SimpleMessagingHandler::SimpleMessagingHandler(const string &url_str)
	: super(),
	broker_url(url_str)
{
	logger(debug) << "Creating a handler for " << url_str;
}

SimpleMessagingHandler::~SimpleMessagingHandler() {

}


void SimpleMessagingHandler::on_start(event &e) {
	logger(debug) << "Starting messaging handler";
	connection &conn = e.container().connect(broker_url);
	logger(debug) << "Connected to " << broker_url.path();
}

void SimpleMessagingHandler::on_sendable(event &e) {
	connection &conn = e.connection();
	
	logger(debug) << "Creating sender for " << broker_url.path();
	conn.create_sender(broker_url.path());
	
	logger(debug) << "Ready to send";
}

void SimpleMessagingHandler::on_message(event &e) {
	logger(debug) << "Ready to receive";
}

void SimpleMessagingHandler::on_accepted(event &e) {
	logger(debug) << "Accepted";
	e.connection().close();
}

void SimpleMessagingHandler::on_connection_closed(event &e) {
	logger(debug) << "Closed";
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */