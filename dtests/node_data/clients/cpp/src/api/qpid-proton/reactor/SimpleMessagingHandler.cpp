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



SimpleMessagingHandler::SimpleMessagingHandler(const string &url_str)
	: super(),
	broker_url(url_str)
{

}

SimpleMessagingHandler::~SimpleMessagingHandler() {

}


void SimpleMessagingHandler::on_start(event &e) {
	connection &conn = e.container().connect(broker_url);

	conn.create_sender(broker_url.path());
}

void SimpleMessagingHandler::on_sendable(event &e) {
	message m;

	m.body("Hello World!");
	
	e.sender().send(m);
	e.sender().close();
}

void SimpleMessagingHandler::on_message(event &e) {

}

void SimpleMessagingHandler::on_accepted(event &e) {
	e.connection().close();
}

void SimpleMessagingHandler::on_connection_closed(event &e) {

}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */