/* 
 * File:   Logger.h
 * Author: opiske
 *
 * Created on May 6, 2015, 7:12 PM
 */

#ifndef LOGGER_H
#define	LOGGER_H

#include <string>

#include "output/Printer.h"

using std::string;

namespace dtests { 
namespace common {
namespace log {   
    

 enum LogLevel {
        none,
        trace,
        debug,
        info,
        warning,
        error
    };

} } }

using dtests::common::log::LogLevel;

class LoggerWrapper;

/**
 * Implements a simple logger
 */
class Logger {
    friend class LoggerWrapper;
    
public:
   
    
    struct LogConfiguration {
        LogLevel minimumLevel;
        FILE *stream;
    };
    
   
    virtual ~Logger(); 
    
    static void initLogger(LogConfiguration logConfiguration);
    
    /**
     * Prints a signed integer
     * @param val Value
     * @returns A reference to this
    */
    Logger &operator<<(int val);
		
    /**
     * Prints an unsigned integer
     * @param val Value
     * @returns A reference to this
     */
    Logger &operator<<(unsigned int val);

    /**
     * Prints a signed long integer
     * @param val Value
     * @returns A reference to this
    */
    Logger &operator<<(long int val);

    /**
     * Prints an unsigned long integer
     * @param val Value
     * @returns A reference to this
    */
    Logger &operator<<(unsigned long int val);

    /**
     * Prints a double
     * @param d Value
     * @returns A reference to this
    */
    Logger &operator<<(double d);

    /**
     * Prints a char
     * @param c Char
     * @returns A reference to this
    */
    Logger &operator<<(char c);

    /**
     * Prints a string
     * @param str String
     * @returns A reference to this
    */
    Logger &operator<<(const string &str);

    /**
     * Prints the address of a pointer
     * @param ptr The address of the pointer
     * @returns A reference to this
    */
    Logger &operator<<(const void *ptr);

    /**
     * Prints a char * string
     * @param str A pointer to a null terminated string
     * @returns A reference to this prevents compiler casting a 
     * regular, temporary, string to a const void pointer, which 
     * would cause the operator for a  const void * to be called
     */
    Logger &operator<<(const char *str);
    
    
    Logger operator()(LogLevel level);
    
private:
    static LogConfiguration logConfiguration;
    LogLevel level;
    Printer printer;
    
     Logger(LogLevel level);

};

#endif	/* LOGGER_H */

