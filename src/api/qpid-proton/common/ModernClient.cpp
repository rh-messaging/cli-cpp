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

using std::string;
using namespace dtests::common::log; 

ModernClient::ModernClient()
	: super()
{
	// TODO Auto-generated constructor stub

}

ModernClient::~ModernClient() {
	// TODO Auto-generated destructor stub
}

void ModernClient::logInfo() const {
	Logger::LogConfiguration logConfiguration; 
	
	logConfiguration.minimumLevel = info;
	logConfiguration.stream = stdout;
	
	LoggerWrapper::initLogger(logConfiguration);
}

void ModernClient::logDebug() const {
	Logger::LogConfiguration logConfiguration; 
	
	logConfiguration.minimumLevel = debug;
	logConfiguration.stream = stdout;
	
	LoggerWrapper::initLogger(logConfiguration);
}

void ModernClient::logTrace() const {
	Logger::LogConfiguration logConfiguration; 
	
	logConfiguration.minimumLevel = trace;
	logConfiguration.stream = stdout;
	
	LoggerWrapper::initLogger(logConfiguration);
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
