/*
 * SimpleMessagingHandler.h
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SIMPLEMESSAGINGHANDLER_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SIMPLEMESSAGINGHANDLER_H_

#include <proton/cpp/Container.h>
#include <proton/cpp/Event.h>
#include <proton/cpp/MessagingEvent.h>

#include <proton/url.h>

using proton::reactor::MessagingHandler;
using proton::reactor::Event;
using proton::reactor::Message;

namespace dtests {
namespace proton {
namespace reactor {

using std::string;


class SimpleMessagingHandler: public MessagingHandler {
public:
	SimpleMessagingHandler(const string &url);
	virtual ~SimpleMessagingHandler();

	void onStart(Event &e);
	void onSendable(Event &e);
	void onMessage(Event &e);
	void onAccepted(Event &e);
	void onConnectionClosed(Event &e);

private:
	typedef MessagingHandler super;

	string url;

	// Connection conn;

	string getHost() const;
};



} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SIMPLEMESSAGINGHANDLER_H_ */
