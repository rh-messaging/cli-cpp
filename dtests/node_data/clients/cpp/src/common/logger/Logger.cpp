/* 
 * File:   Logger.cpp
 * Author: opiske
 * 
 * Created on May 6, 2015, 7:12 PM
 */

#include "Logger.h"

using namespace dtests::common::log;

Logger::LogConfiguration Logger::logConfiguration = {
    debug, stdout
};

Logger::Logger(LogLevel level)
    : level(level),
    printer(Logger::logConfiguration.stream)
{
    printer.flush();
}

Logger::~Logger()
{
    if (level >= logConfiguration.minimumLevel) {
        printer.print('\n');
    }
}

void Logger::initLogger(LogConfiguration logConfiguration)
{
    Logger::logConfiguration = logConfiguration;

}

Logger &Logger::operator<<(int val)
{
    if (level >= logConfiguration.minimumLevel) {
        printer.print(val);
    }

    return *this;
}

Logger &Logger::operator<<(unsigned int val)
{

    if (level >= logConfiguration.minimumLevel) {
        printer.print(val);
    }

    return *this;
}

Logger &Logger::operator<<(long int val)
{

    if (level >= logConfiguration.minimumLevel) {
        printer.print(val);
    }

    return *this;
}

Logger &Logger::operator<<(unsigned long int val)
{

    if (level >= logConfiguration.minimumLevel) {
        printer.print(val);
    }

    return *this;
}

Logger &Logger::operator<<(double d)
{

    if (level >= logConfiguration.minimumLevel) {
        printer.print(d);
    }

    return *this;
}

Logger &Logger::operator<<(char c)
{

    if (level >= logConfiguration.minimumLevel) {
        printer.print(c);
    }

    return *this;
}

Logger &Logger::operator<<(const string &str)
{

    if (level >= logConfiguration.minimumLevel) {
        printer.print(str.c_str());
    }

    return *this;
}

Logger &Logger::operator<<(const void *ptr)
{

    if (level >= logConfiguration.minimumLevel) {
        printer.print(ptr);
    }

    return *this;
}

Logger &Logger::operator<<(const char *str)
{

    if (level >= logConfiguration.minimumLevel) {
        printer.print(str);
    }

    return *this;
}

Logger Logger::operator()(LogLevel logLevel)
{
    Logger ret = Logger(logLevel);

    switch (logLevel) {
    case none:
    {
        break;
    }
    case trace:
    {
        ret << "[trace]: ";
        break;
    }
    case debug:
    {
        ret << "[debug]: ";
        break;
    }
    case info:
    {
        ret << "[info ]: ";
        break;
    }
    case warning:
    {
        ret << "[warn ]: ";
        break;
    }
    case error:
    {
        ret << "[error]: ";
        break;
    }
    }

    return ret;
}

