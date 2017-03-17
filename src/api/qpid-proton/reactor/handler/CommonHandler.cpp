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


CommonHandler::CommonHandler(
    const string &url_str,
    string user,
    string password,
    string sasl_mechanisms,
    int timeout,
    string conn_reconnect,
    int32_t conn_reconnect_interval,
    int32_t conn_reconnect_limit,
    int32_t conn_reconnect_timeout,
    uint32_t conn_reconnect_first,
    uint32_t conn_reconnect_increment,
    bool conn_reconnect_doubling,
    bool conn_reconnect_custom,
    uint32_t conn_heartbeat,
    uint32_t max_frame_size,
    string log_msgs
)
    : super(),
    broker_url(url_str),
    user(user),
    password(password),
    sasl_mechanisms(sasl_mechanisms),
    conn_reconnect(conn_reconnect),
    conn_reconnect_interval(conn_reconnect_interval),
    conn_reconnect_limit(conn_reconnect_limit),
    conn_reconnect_timeout(conn_reconnect_timeout),
    conn_reconnect_first(conn_reconnect_first),
    conn_reconnect_increment(conn_reconnect_increment),
    conn_reconnect_doubling(conn_reconnect_doubling),
    conn_reconnect_custom(conn_reconnect_custom),
    conn_heartbeat(conn_heartbeat),
    max_frame_size(max_frame_size),
    log_msgs(log_msgs),
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
