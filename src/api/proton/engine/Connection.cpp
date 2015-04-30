/*
 * Connection.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */

#include "Connection.h"

using namespace std;

const string Connection::DEFAULT_PORT = "5672";

Connection::Connection()
	: driver(NULL),
	  connector(NULL),
	  connection(NULL)
{

}

Connection::Connection(const string &hostname, const string &port)
	: driver(NULL),
	  connector(NULL),
	  connection(NULL)
{
	init(hostname, port);
}

Connection::~Connection() {
	pn_connection_free(connection);
	pn_connector_free(connector);
	pn_driver_free(driver);
}


void Connection::init(const string &hostname, const string &port) {
	BOOST_LOG_TRIVIAL(debug) << "Initializing the driver";

	driver = pn_driver();
	if (driver == NULL) {
		BOOST_LOG_TRIVIAL(error) << "Unable to initialize driver";
	}

	BOOST_LOG_TRIVIAL(debug) << "Initializing the connector";
	connector = pn_connector(driver, hostname.c_str(), port.c_str(), NULL);
	if (connector == NULL) {
		BOOST_LOG_TRIVIAL(error) << "Failed to initialize the connector";
	}

	connection = pn_connection();
	if (connection == NULL) {
		BOOST_LOG_TRIVIAL(error) << "Unable to initialize the connection";
	}

	pn_connector_set_connection(connector, connection);
	BOOST_LOG_TRIVIAL(error) << "Successfully initialized the connection";
}


void Connection::open() {
	pn_connection_open(connection);
}

void Connection::waitForDriver(int timeout) {
	pn_driver_wait(driver, timeout);
}


pn_driver_t* Connection::getDriver() const {
	return driver;
}


pn_connection_t *Connection::getConnection() const {
	return connection;
}




