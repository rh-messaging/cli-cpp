/*
 * ControlOptions.cpp
 *
 *  Created on: Apr 16, 2015
 *      Author: opiske
 */


#include "options/modern/SenderOptionsParser.h"

using namespace dtests::common;

SenderOptionsParser::SenderOptionsParser()
    : super()
{
    add_option("--msg-content")
            .dest("msg-content")
            .help("specify content");

    add_option("-c", "--count")
            .dest("count")
            .type("int")
            .help("stop after count messages")
            .metavar("COUNT");

    add_option("--msg-correlation-id")
            .dest("msg-correlation-id")
            .help("message correlation id")
            .metavar("ID");

    add_option("--msg-reply-to")
            .dest("msg-reply-to")
            .help("specify reply-to address");

    add_option("--msg-user-id")
            .dest("msg-user-id")
            .help("message user id");

    add_option("--msg-ttl")
            .dest("msg-ttl")
            .help("message time-to-live (ms)");

    add_option("--msg-id")
            .dest("msg-id")
            .help("use the supplied id instead of generating one");

    add_option("--msg-priority")
            .dest("msg-priority")
            .type("short")
            .help("message priority");

    add_option("--msg-content-type")
            .dest("msg-content-type")
            .help("message content type field");

    add_option("--msg-durable")
            .dest("msg-durable")
            .help("send durable messages yes/no");

    add_option("--msg-subject")
            .dest("msg-subject")
            .help("specify message subject");

    add_option("--msg-content-from-file")
            .dest("msg-content-from-file")
            .help("specify file name to load content from");

    add_option("--msg-properties")
            .dest("msg-properties")
            .help("specify message properties");

#ifdef ENABLE_BLOCKING
    add_option("--tx-size")
            .dest("tx-size")
            .type("int")
            .set_default(Messenger::DEFAULT_TX_SIZE)
            .help("transactional mode: batch message count size " \
						"(negative skips tx-action before exit)");

#endif // ENABLE_BLOCKING

    add_option("--timeout")
            .dest("timeout")
            .help("exit after the specified time")
            .type("int");
    
    add_option("--log-msgs")
            .dest("log-msgs")
            .help("log message options (currently ignored)");
}

SenderOptionsParser::~SenderOptionsParser()
{
}

void SenderOptionsParser::validate(const Values& options) const
{
    super::validate(options);
}
