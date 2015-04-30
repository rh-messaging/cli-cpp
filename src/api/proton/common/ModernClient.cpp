/*
 * ModernClient.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#include "ModernClient.h"

namespace dtests {
namespace proton {
namespace common {

namespace logging = boost::log;

using std::string;

ModernClient::ModernClient()
	: super()
{
	// TODO Auto-generated constructor stub

}

ModernClient::~ModernClient() {
	// TODO Auto-generated destructor stub
}

void ModernClient::logInfo() const {
	logging::core::get()->set_filter
	(
	        logging::trivial::severity >= logging::trivial::info
	);
}

void ModernClient::logDebug() const {
	logging::core::get()->set_filter
	(
	        logging::trivial::severity >= logging::trivial::debug
	);
}

void ModernClient::logTrace() const {
	logging::core::get()->set_filter
	(
	        logging::trivial::severity >= logging::trivial::trace
	);
}

void ModernClient::setLogLevel(const optparse::Values &options) const {
	logTrace();
	if (options.is_set("client-log-level")) {
		const string logLevel = options["client-log-level"];

		if (logLevel == "info") {
			logInfo();
		}
		else {
			if (logLevel == "debug") {
				logDebug();
			}
		}
	}

}


} /* namespace messenger */
} /* namespace proton */
} /* namespace dtests */
