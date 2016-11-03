/*
 * ControlOptions.cpp
 *
 *  Created on: Apr 16, 2015
 *      Author: opiske
 */


#include "options/modern/ReceiverOptionsParser.h"

using namespace dtests::common;

ReceiverOptionsParser::ReceiverOptionsParser()
    : super()
{
    add_option("-c", "--count")
            .dest("count")
            .type("int")
            .help("stop after COUNT messages")
            .metavar("COUNT");

#ifdef ENABLE_BLOCKING
    add_option("--tx-size")
            .dest("tx-size")
            .help("transactional mode: batch message count size " \
							"(negative skips tx-action before exit)");
#endif // ENABLE_BLOCKING

    add_option("--timeout")
            .dest("timeout")
            .help("exit after the specified TIME (seconds)")
            .type("int")
            .metavar("TIME");
    
    add_option("--log-msgs")
            .dest("log-msgs")
            .help("log message OPTIONS (currently ignored)")
            .metavar("OPTIONS");

    add_option("--action")
            .dest("msg-action")
            .help("ACTION on acquired message (default: no-action)")
            .metavar("ACTION");
}

ReceiverOptionsParser::~ReceiverOptionsParser()
{
}

void ReceiverOptionsParser::validate(const Values& options) const
{
    super::validate(options);
}
