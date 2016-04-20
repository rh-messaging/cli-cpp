/*
 * CommonHandler.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#include "CommonHandler.h"

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;
using namespace dtests::common::log;

Logger CommonHandler::logger = LoggerWrapper::getLogger();


CommonHandler::CommonHandler(const string &url_str, int timeout)
    : super(),
    broker_url(url_str),
    timeoutTask(NULL), timer(timeout, "timeout")
{
    logger(debug) << "Creating a handler for " << url_str;
}

CommonHandler::~CommonHandler()
{
    logger(debug) << "Destroying the common handler";
}


void CommonHandler::setupTimer(event &e) {
    logger(debug) << "Setting up timeout";
    
    task t = e.container().schedule(1000);
    timeoutTask = &t;
}

void CommonHandler::timerEvent(event& e) {
    if (!timeoutTask) {
        logger(debug) << "Quiescing, therefore ignoring event: " << e.name();

        return;
    }

    if (timer.isExpired()) {
        logger(info) << "Timed out";

        
        /**
         * TODO: this is, certainly, a bad way to exit. However, upstream does 
         * not yet have a stable interface for timers. This should be fixed in 
         * the future.
         */
        exit(1);
    } else {
        timer--;
        logger(debug) << "Waiting ...";
        e.container().schedule(1000);
    }
}

void CommonHandler::disableTimer() {
    timeoutTask = NULL;
}


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */