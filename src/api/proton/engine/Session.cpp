/*
 * Session.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */

#include "Session.h"

Session::Session():
	session(NULL)
{

}


Session::Session(const Connection &connection)
	: session(NULL)
{
	BOOST_LOG_TRIVIAL(debug) << "Initializing the session";

	session = pn_session(connection.getConnection());
	if (session == NULL) {
		BOOST_LOG_TRIVIAL(error) << "Failed to initialize the session";
	}

	BOOST_LOG_TRIVIAL(debug) << "Successfully initialized the session";
}


Session::~Session() {
	pn_session_free(session);
}

void Session::open() const {
	pn_session_open(session);
}

pn_session_t *Session::getSession() const {
	return session;
}
