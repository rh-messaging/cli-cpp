/*
 * CommonHandler.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#include <proton/container.hpp>

#include "CommonHandler.h"

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;
using namespace dtests::common::log;

Logger CommonHandler::logger = LoggerWrapper::getLogger();


CommonHandler::CommonHandler(const string &url_str, string user, string password, string sasl_mechanisms, int timeout, string conn_reconnect, uint32_t max_frame_size)
    : super(),
    broker_url(url_str),
    user(user),
    password(password),
    sasl_mechanisms(sasl_mechanisms),
    conn_reconnect(conn_reconnect),
    max_frame_size(max_frame_size),
    cont(NULL),
    timer(timeout, "timeout")
{
    logger(debug) << "Creating a handler for " << url_str;
}

CommonHandler::~CommonHandler()
{
    logger(debug) << "Destroying the common handler";
}


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
