/*
 * ReceiverOptionsParser.cpp
 *
 *  Created on: Apr 16, 2015
 *      Author: opiske
 */

#include <cstdlib>

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
            .help("log message OPTIONS")
            .metavar("OPTIONS");

    add_option("--action")
            .dest("msg-action")
            .help("ACTION on acquired message (default: no-action)")
            .metavar("ACTION");

    add_option("--action-size")
            .dest("msg-action-size")
            .help("related action is applied in the batch of given SIZE (default: 1)")
            .type("int")
            .metavar("SIZE");

    add_option("--process-reply-to")
            .dest("process-reply-to")
            .help("send message to reply-to address if enabled and message got reply-to address")
            .type("bool")
            .action("store_true");

    add_option("--recv-browse")
            .dest("recv-browse")
            .help("browsing receiver if enabled")
            .type("bool")
            .action("store_true");
}

ReceiverOptionsParser::~ReceiverOptionsParser()
{
}

void ReceiverOptionsParser::validate(const Values& options) const
{
    super::validate(options);

    if(options.is_set("msg-action-size") && (atoi(options["msg-action-size"].c_str()) < 1)) {
        print_help();
        error("action-size must not be less than 1");
    }
}
