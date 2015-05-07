/* 
 * File:   ut_logger_test.cpp
 * Author: opiske
 *
 * Created on May 7, 2015, 1:05 PM
 */

#include <cstdlib>

#include "logger/Logger.h"
#include "TestUtils.h"

using namespace dtests::common::log;

/*
 * This test asserts whether the code can output to the stdout (test pass if it
 * doesn't crash)
 */
int main(int argc, char** argv) {
	Logger::LogConfiguration logConfiguration; 
	
	logConfiguration.minimumLevel = debug;
	logConfiguration.stream = stdout;
	
	
	Logger::initLogger(logConfiguration);
	
	Logger logger = Logger(debug);
	
	logger(debug) << "This is a test"; 
	logger(trace) << "This shall not appear"; 
	logger(info) << "This is another log message";
	logger(error) << "And another log message";

	return 0;
}

