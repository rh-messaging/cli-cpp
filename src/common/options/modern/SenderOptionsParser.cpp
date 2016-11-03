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
            .help("specify CONTENT of message")
            .metavar("CONTENT");

    add_option("-c", "--count")
            .dest("count")
            .type("int")
            .help("stop after COUNT messages")
            .metavar("COUNT");

    add_option("--msg-correlation-id")
            .dest("msg-correlation-id")
            .help("message correlation ID")
            .metavar("ID");

    add_option("--msg-reply-to")
            .dest("msg-reply-to")
            .help("specify reply-to ADDRESS")
            .metavar("ADDRESS");

    add_option("--msg-user-id")
            .dest("msg-user-id")
            .help("message user ID")
            .metavar("ID");

    add_option("--msg-ttl")
            .dest("msg-ttl")
            .help("message Time-To-Live (ms)")
            .metavar("TTL");

    add_option("--msg-id")
            .dest("msg-id")
            .help("use the supplied ID instead of generating one")
            .metavar("ID");

    add_option("--msg-priority")
            .dest("msg-priority")
            .type("short")
            .help("message PRIORITY")
            .metavar("PRIORITY");

    add_option("--msg-content-type")
            .dest("msg-content-type")
            .help("message content TYPE field")
            .metavar("TYPE");

    add_option("--msg-durable")
            .dest("msg-durable")
            .help("send durable messages (yes/no)")
            .metavar("DURABLE");

    add_option("--msg-subject")
            .dest("msg-subject")
            .help("specify message SUBJECT")
            .metavar("SUBJECT");

    add_option("--msg-content-from-file")
            .dest("msg-content-from-file")
            .help("specify FILENAME to load content from")
            .metavar("FILENAME");

    add_option("--msg-properties")
            .dest("msg-properties")
            .help("specify message PROPERTIES")
            .metavar("PROPERTIES");

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
            .help("exit after the specified TIME (seconds)")
            .type("int")
            .metavar("TIME");
    
    add_option("--log-msgs")
            .dest("log-msgs")
            .help("log message OPTIONS (currently ignored)")
            .metavar("OPTIONS");
}

SenderOptionsParser::~SenderOptionsParser()
{
}

void SenderOptionsParser::validate(const Values& options) const
{
    super::validate(options);
}
