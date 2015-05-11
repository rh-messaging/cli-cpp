/* 
 * File:   LoggerFactory.cpp
 * Author: opiske
 * 
 * Created on May 11, 2015, 11:13 AM
 */

#include "LoggerWrapper.h"

bool LoggerWrapper::initialized = false;
Logger LoggerWrapper::logger = Logger(dtests::common::log::none);

LoggerWrapper::LoggerWrapper() { }

LoggerWrapper::LoggerWrapper(const LoggerWrapper& orig) { }

LoggerWrapper::~LoggerWrapper() { }


void LoggerWrapper::initLogger(Logger::LogConfiguration configuration) {
	
	if (!initialized) { 
		Logger::initLogger(configuration);
		initialized = true;
	}
	
};

const Logger &LoggerWrapper::getLogger() {
	return LoggerWrapper::logger;
}