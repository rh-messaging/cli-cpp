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
            .help("stop after count messages")
            .metavar("COUNT");

#ifdef ENABLE_BLOCKING
    add_option("--tx-size")
            .dest("tx-size")
            .help("transactional mode: batch message count size " \
							"(negative skips tx-action before exit)");
#endif // ENABLE_BLOCKING

    add_option("--timeout")
            .dest("timeout")
            .help("exit after the specified time")
            .type("int");
}

ReceiverOptionsParser::~ReceiverOptionsParser()
{
}

void ReceiverOptionsParser::validate(const Values& options) const
{
    super::validate(options);
}
