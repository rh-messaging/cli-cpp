/* 
 * File:   LoggerFactory.h
 * Author: opiske
 *
 * Created on May 11, 2015, 11:13 AM
 */

#ifndef LOGGERFACTORY_H
#define LOGGERFACTORY_H

#include "Logger.h"

class LoggerWrapper {
  public:
    virtual ~LoggerWrapper();

    static void initLogger(Logger::LogConfiguration configuration);
    static const Logger &getLogger();

  private:
    static bool initialized;
    static Logger logger;

    LoggerWrapper();
    LoggerWrapper(const LoggerWrapper& orig);

};

#endif /* LOGGERFACTORY_H */

