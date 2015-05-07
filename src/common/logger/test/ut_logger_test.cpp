/* 
 * File:   ut_logger_test.cpp
 * Author: opiske
 *
 * Created on May 7, 2015, 1:05 PM
 */

#include <cstdlib>

#include "logger/Logger.h"
#include "TestUtils.h"

using dtests::common::log::info;
using dtests::common::log::debug;

/*
 * 
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
	
	Logger::initLogger(logConfiguration);
	
	Logger logger = Logger(debug);
	
	
	logger << "This is a test"; 
	
	fclose(logConfiguration.stream);
	size_t len = strlen("This is a test");
	
	bool ret = assertFileContent(logFileName, "This is a test", len);
	if (ret != 0) {
		return 1;
	}

	return 0;
}

