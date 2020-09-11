/* 
 * File:   ut_logger_test.cpp
 * Author: opiske
 *
 * Created on May 7, 2015, 1:05 PM
 */

#include <cstdlib>

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"
#include "TestUtils.h"

using dtests::common::log::info;
using dtests::common::log::debug;

/*
 * This test checks the ability to log a single message to a file
 */
int main(int argc, char** argv) {
	const char *logFileName = "ut_logger_test.log";
	Logger::LogConfiguration logConfiguration; 
	
	logConfiguration.minimumLevel = info;
	logConfiguration.stream = fopen(logFileName, "w+");
	
	if (logConfiguration.stream == NULL) {
		fprintf(stderr, "Unable to open test log file\n");
		return 1;
	}
	
	// Logger::initLogger(logConfiguration);
	LoggerWrapper::initLogger(logConfiguration);
	
	Logger logger = LoggerWrapper::getLogger();
	
	logger(info) << "This is a test"; 
	
	fclose(logConfiguration.stream);
	
	const char *message = "[info ]: This is a test";
	size_t len = strlen(message);
	
	bool ret = assertFileContent(logFileName, message, len);
	if (ret != 0) {
		return 1;
	}

	return 0;
}

