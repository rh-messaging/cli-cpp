/*
 * Client.h
 *
 *  Created on: Apr 28, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_CLIENT_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_CLIENT_H_

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

/**
 * Base class for implementing the main logic for messaging clients.
 */
class Client {
public:
	Client();
	virtual ~Client();

        /**
         * Run the client
         * @param argc argument count
         * @param argv argument value
         * @return EXIT_SUCCESS if executed successfully or EXIT_FAILURE 
         * otherwise
         */
	virtual int run(int argc, char **argv) const = 0;
        
protected:
        static Logger logger;
};

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_CLIENT_H_ */
