/*
 * Session.h
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_ENGINE_SESSION_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_ENGINE_SESSION_H_

#include <proton/connection.h>
#include <proton/session.h>

#include <boost/log/trivial.hpp>

#include "Connection.h"

class Session {
friend class Link;


public:
	Session();
	Session(const Connection &connection);
	virtual ~Session();

	void open() const;

private:
	pn_session_t *session;

	pn_session_t *getSession() const;
};

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_ENGINE_SESSION_H_ */
