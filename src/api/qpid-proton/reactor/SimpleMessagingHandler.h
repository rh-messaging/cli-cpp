/*
 * SimpleMessagingHandler.h
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SIMPLEMESSAGINGHANDLER_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SIMPLEMESSAGINGHANDLER_H_

#include <proton/container.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/connection.hpp>

#include <proton/url.hpp>

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

using proton::messaging_handler;
using proton::message;
using proton::event;
using proton::connection;
using proton::sender;
using proton::url;

namespace dtests {
namespace proton {
namespace reactor {

using std::string;


class SimpleMessagingHandler: public messaging_handler {
public:
	SimpleMessagingHandler(const string &url);
	virtual ~SimpleMessagingHandler();

	void on_start(event &e);
	void on_sendable(event &e);
	void on_message(event &e);
	void on_accepted(event &e);
	void on_connection_closed(event &e);

protected:
	static Logger logger;
	url broker_url;
	
private:
	typedef messaging_handler super;

        
	
};



} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SIMPLEMESSAGINGHANDLER_H_ */
