/*
 * SimpleMessagingHandler.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#include "SimpleMessagingHandler.h"

using proton::reactor::MessagingHandler;
using proton::reactor::Event;
using proton::reactor::Message;
using proton::reactor::Connection;

namespace dtests {
namespace proton {
namespace reactor {



SimpleMessagingHandler::SimpleMessagingHandler(const string &url)
	: super(),
	  url(url)
	{


}

SimpleMessagingHandler::~SimpleMessagingHandler() {

}

string SimpleMessagingHandler::getHost() const {
	pn_url_t *urlPtr = pn_url_parse(url.c_str());

	if (!urlPtr) {
		//pn_incref(pnUrl); TODO: handle error
	}

	pn_incref(urlPtr);
	const char *host = pn_url_get_host(urlPtr);

	string ret = string("localhost");
	if (host) {
		ret = string(host);
	}

	pn_decref(urlPtr);
	return ret;
}

void SimpleMessagingHandler::onStart(Event &e) {
	string host = getHost();

	Connection conn = e.getContainer().connect(host);
	e.getContainer().createSender(url);
}

void SimpleMessagingHandler::onSendable(Event &e) {
	Message m;

	m.setBody("Hello World!");
	e.getSender().send(m);
	e.getSender().close();
}

void SimpleMessagingHandler::onMessage(Event &e) {

}

void SimpleMessagingHandler::onAccepted(Event &e) {
	e.getConnection().close();
}

void SimpleMessagingHandler::onConnectionClosed(Event &e) {

}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
