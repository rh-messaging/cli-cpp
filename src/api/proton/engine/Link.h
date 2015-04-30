/*
 * Link.h
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_ENGINE_LINK_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_ENGINE_LINK_H_

#include <proton/connection.h>
#include <proton/session.h>
#include <proton/terminus.h>
#include <proton/link.h>
#include <proton/error.h>
#include <proton/engine.h>

#include <boost/log/trivial.hpp>

#include <string>
#include <iostream>

#include "Session.h"

using namespace std;

class Link {
public:
	Link();
	Link(const Session &session);
	virtual ~Link();

	void open();

	void send();

protected:
	pn_link_t *link;
	pn_terminus_t *terminus;

	void init(const Session &session);
};

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_ENGINE_LINK_H_ */
