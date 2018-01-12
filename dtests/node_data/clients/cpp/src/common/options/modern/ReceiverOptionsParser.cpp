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

    add_option("--durable-subscriber")
            .dest("durable-subscriber")
            .help("create durable subscription to topic (true/false, default: false)")
            .metavar("ENABLED");

    add_option("--subscriber-unsubscribe")
            .dest("subscriber-unsubscribe")
            .help("unsubscribe durable subscriptor with given name (true/false, default: false, provide --durable-subscriber-name)")
            .metavar("UNSUBSCRIBE");

    add_option("--durable-subscriber-prefix")
            .dest("durable-subscriber-prefix")
            .help("prefix to use to identify this connection subscriber")
            .metavar("PREFIX");

    add_option("--durable-subscriber-name")
            .dest("durable-subscriber-name")
            .help("name of the durable subscriber to be unsubscribe")
            .metavar("NAME");

    add_option("--shared-subscriber")
            .dest("shared-subscriber")
            .help("create shared subscription to topic (true/false, default: false)")
            .metavar("SHARED");
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

    if (options.is_set("durable-subscriber")) {
        std::string durable_subscriber_lower = options["durable-subscriber"];
        std::transform(durable_subscriber_lower.begin(), durable_subscriber_lower.end(), durable_subscriber_lower.begin(), ::tolower);

        if (durable_subscriber_lower != "true" && durable_subscriber_lower != "false") {
            print_help();
            std::stringstream sstm;
            sstm << "Value \"" << options["durable-subscriber"] << "\" is not valid for --durable-subscriber option, must be one of true/false";
            error(sstm.str());
        }
    }

    if (options.is_set("subscriber-unsubscribe")) {
        std::string subscriber_unsubscribe_lower = options["subscriber-unsubscribe"];
        std::transform(subscriber_unsubscribe_lower.begin(), subscriber_unsubscribe_lower.end(), subscriber_unsubscribe_lower.begin(), ::tolower);

        if (subscriber_unsubscribe_lower != "true" && subscriber_unsubscribe_lower != "false") {
            print_help();
            std::stringstream sstm;
            sstm << "Value \"" << options["subscriber-unsubscribe"] << "\" is not valid for --subscriber-unsubscribe option, must be one of true/false";
            error(sstm.str());
        }
    }

    if (options.is_set("shared-subscriber")) {
        std::string shared_subscriber_lower = options["shared-subscriber"];
        std::transform(shared_subscriber_lower.begin(), shared_subscriber_lower.end(), shared_subscriber_lower.begin(), ::tolower);

        if (shared_subscriber_lower != "true" && shared_subscriber_lower != "false") {
            print_help();
            std::stringstream sstm;
            sstm << "Value \"" << options["shared-subscriber"] << "\" is not valid for --shared-subscriber option, must be one of true/false";
            error(sstm.str());
        }
    }
}
