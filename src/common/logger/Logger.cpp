/* 
 * File:   Logger.cpp
 * Author: opiske
 * 
 * Created on May 6, 2015, 7:12 PM
 */

#include "Logger.h"

using dtests::common::log::debug;

Logger::LogConfiguration Logger::logConfiguration = { 
	.minimumLevel = debug,
	.stream = stdout 
};

Logger::Logger(LogLevel level)
	: level(level),
	printer(Logger::logConfiguration.stream)
{

}

Logger::~Logger() {
}

void Logger::initLogger(LogConfiguration logConfiguration) {
	Logger::logConfiguration = logConfiguration;

}

Logger &Logger::operator<<(int val) {
	if (level >= logConfiguration.minimumLevel) { 
		printer.print(val);
	}
	
	return *this;
}


Logger &Logger::operator<<(unsigned int val) {
	
	if (level >= logConfiguration.minimumLevel) { 
		printer.print(val);
	}
	
	return *this;
}


Logger &Logger::operator<<(long int val) {
	
	if (level >= logConfiguration.minimumLevel) { 
		printer.print(val);
	}
	
	return *this;
}


Logger &Logger::operator<<(unsigned long int val) {
	
	if (level >= logConfiguration.minimumLevel) { 
		printer.print(val);
	}
	
	return *this;
}


Logger &Logger::operator<<(double d) {
	
	if (level >= logConfiguration.minimumLevel) { 
		printer.print(d);
	}
	
	return *this;
}


Logger &Logger::operator<<(char c) {
	
	if (level >= logConfiguration.minimumLevel) { 
		printer.print(c);
	}
	
	return *this;
}

Logger &Logger::operator<<(const string &str) {
	
	if (level >= logConfiguration.minimumLevel) { 
		printer.print(str.c_str());
	}
	
	return *this;
}


Logger &Logger::operator<<(const void *ptr) {
	
	if (level >= logConfiguration.minimumLevel) { 
		printer.print(ptr);
	}
	
	return *this;
}


Logger &Logger::operator<<(const char *str) {
	
	if (level >= logConfiguration.minimumLevel) { 
		printer.print(str);
	}
	
	return *this;
}

