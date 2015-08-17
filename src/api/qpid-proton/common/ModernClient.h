/*
 * ModernClient.h
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_MODERNCLIENT_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_MODERNCLIENT_H_

#include <optparse/OptionParser.h>

#include <string>

#include "Client.h"
#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

namespace dtests {
namespace proton {
namespace common {

/**
 * An implementation of a the CPP client that uses the newer format for command
 * line options, logger, etc.
 */
class ModernClient: public Client {
public:
	ModernClient();
	virtual ~ModernClient();

	virtual int run(int argc, char **argv) const = 0;

protected:
        /**
         * Given the command line options, it sets the log level accordingly
         * @param options the command line options
         */
	void setLogLevel(const optparse::Values &options) const;

private:
	typedef Client super;

	void logInfo() const;
	void logDebug() const;
	void logTrace() const;



};

} /* namespace messenger */
} /* namespace proton */
} /* namespace dtests */

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_MODERNCLIENT_H_ */
