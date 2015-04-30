/*
 * Connection.h
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_ENGINE_CONNECTION_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_ENGINE_CONNECTION_H_

#include <proton/driver.h>
#include <proton/connection.h>
#include <proton/transport.h>

#include <boost/log/trivial.hpp>

#include <string>

using namespace std;

class Connection {

/**
 * This is required so that we can encapsulate the code within the proton API,
 * yet still allow Session object to be created from a connection - thus
 * mimicking the lower level API behavior
 */

friend class Session;

public:
	static const string DEFAULT_PORT;
	static const int TIMEOUT_UNLIMITED = -1;

	Connection();
	Connection(const string &hostname, const string &port = DEFAULT_PORT);
	virtual ~Connection();

	void open();
	void waitForDriver(int timeout = TIMEOUT_UNLIMITED);


private:
	pn_driver_t *driver;
	pn_connector_t *connector;
	pn_connection_t *connection;

	void init(const string &hostname, const string &port);

	pn_connection_t *getConnection() const;
	pn_driver_t *getDriver() const;
};

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_ENGINE_CONNECTION_H_ */
