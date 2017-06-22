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

    add_option("--recv-selector")
            .dest("recv-selector")
            .help("specify message SELECTOR")
            .metavar("SELECTOR");

    add_option("--duration")
            .dest("duration")
            .help("message actions total DURATION (defines msg-rate together with count in seconds)")
            .metavar("DURATION");

    add_option("--duration-mode")
            .dest("duration-mode")
            .help("specifies where to wait to achieve expected duration MODE (after-receive, after-receive-action, after-receive-action-tx-action, default: after-receive)")
            .metavar("MODE");

    add_option("--recv-listen")
            .dest("recv-listen")
            .help("enable receiver listen (P2P) (true/false, default: false)")
            .metavar("LISTEN");

    add_option("--recv-listen-port")
            .dest("recv-listen-port")
            .help("define port for local listening (default: 5672)")
            .type("int")
            .metavar("PORT");
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

    if(options.is_set("duration-mode") && options["duration-mode"] == "after-receive-action-tx-action") {
        error("transactions are not supported yet");
    }
}
