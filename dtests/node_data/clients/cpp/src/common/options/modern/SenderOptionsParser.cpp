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
    callbackProperty = StringAppendCallback();
    callbackList = StringAppendCallback();
    callbackMap = StringAppendCallback();

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
            .help("send durable messages (true/false, default: false)")
            .metavar("DURABLE");

    add_option("--msg-subject")
            .dest("msg-subject")
            .help("specify message SUBJECT")
            .metavar("SUBJECT");

    add_option("--msg-content-from-file")
            .dest("msg-content-from-file")
            .help("specify FILENAME to load content from")
            .metavar("FILENAME");

    add_option("--msg-property")
            .action("callback")
            .callback(callbackProperty)
            .help("specify message PROPERTY (NAME=VALUE|NAME~VALUE, '~' enables type auto-cast)")
            .metavar("PROPERTY");

    add_option("--msg-content-list-item")
            .action("callback")
            .callback(callbackList)
            .help("specify list ENTRY for message body (VALUE|~VALUE, '~' enables type auto-cast)")
            .metavar("ENTRY");

    add_option("--msg-content-map-item")
            .action("callback")
            .callback(callbackMap)
            .help("specify map ENTRY for message body (NAME=VALUE|NAME~VALUE, '~' enables type auto-cast)")
            .metavar("ENTRY");

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
            .help("log message OPTIONS")
            .metavar("OPTIONS");

    add_option("--duration")
            .dest("duration")
            .help("message actions total DURATION (defines msg-rate together with count in seconds)")
            .metavar("DURATION");

    add_option("--duration-mode")
            .dest("duration-mode")
            .help("specifies where to wait to achieve expected duration MODE (before-send, after-send, after-send-tx-action, default: after-send)")
            .metavar("MODE");

    add_option("--msg-group-id")
            .dest("msg-group-id")
            .help("message group ID")
            .metavar("ID");

    add_option("--msg-group-seq")
            .dest("msg-group-seq")
            .help("message group sequence")
            .metavar("SEQUENCE");

    add_option("--msg-reply-to-group-id")
            .dest("reply-to-group-id")
            .help("reply to message group ID")
            .metavar("ID");
}

SenderOptionsParser::~SenderOptionsParser()
{
}

void SenderOptionsParser::validate(const Values& options) const
{
    super::validate(options);

    if(options.is_set("duration-mode") && options["duration-mode"] == "after-send-tx-action") {
        error("transactions are not supported yet");
    }
}
