/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReceivingClient.cpp
 * Author: opiske
 * 
 * Created on October 19, 2015, 4:25 PM
 */

#include <cstdlib>

#include "ReceivingClient.h"

using proton::default_container;

namespace dtests {
namespace proton {
namespace reactor {

using dtests::common::UriParser;

ReceivingClient::ReceivingClient()
    : super()
{
}

ReceivingClient::~ReceivingClient()
{
}

void ReceivingClient::setMessageOptions(const OptionsSetter &setter, message &msg) const
{

}

int ReceivingClient::run(int argc, char **argv) const
{
    const string usage = "usage: %prog [OPTION]... DIR [FILE]...";
    const string version = "1.0";
    const string desc = "C/C++ AMQ reactive API receiver client for Qpid Proton";

    ReceiverOptionsParser parser = ReceiverOptionsParser();
    UriParser uri_parser = UriParser();

    /**
     * WARNING: do not reassign the result of chainned calls to usage/version/etc
     *  because it causes the code to segfault. For example, this crashes:
     *
     *  ControlOptions parser = ControlOptions().usage(usage)
     */
    parser.usage(usage).version(version).description(desc);

    optparse::Values options = parser.parse_args(argc, argv);

    parser.validate(options);

    setLogLevel(options);

    const string address = options["broker-url"];
    
    uri_parser.parse(options["broker-url"]);
    
    string msg_action = "no-action";
    if(options.is_set("msg-action")) {
        msg_action = options["msg-action"];
    }

    int msg_action_size = 1;
    if(options.is_set("msg-action-size")) {
        msg_action_size = atoi(options["msg-action-size"].c_str());
    }

    string user = "";
    if (options.is_set("user")) {
        user = options["user"];
    } else {
        user = uri_parser.getUser();
    }
    
    string password = "";
    if (options.is_set("password")) {
        password = options["password"];
    } else {
        password = uri_parser.getPassword();
    }
    
    string sasl_mechanisms = "";
    if (options.is_set("sasl-mechanisms")) {
        sasl_mechanisms = options["sasl-mechanisms"];
    } else if (user != "" && password != "") {
        sasl_mechanisms = "PLAIN";
    } else {
        sasl_mechanisms = "ANONYMOUS";
    }
    
    bool conn_reconnect_custom = false;

    string conn_reconnect = "true";
    if (options.is_set("conn-reconnect")) {
        conn_reconnect = options["conn-reconnect"];
        std::transform(conn_reconnect.begin(), conn_reconnect.end(), conn_reconnect.begin(), ::tolower);
    }

    int32_t conn_reconnect_interval = -1;
    if (options.is_set("conn-reconnect-interval")) {
        conn_reconnect_interval = std::strtol(options["conn-reconnect-interval"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    int32_t conn_reconnect_limit = -1;
    if (options.is_set("conn-reconnect-limit")) {
        conn_reconnect_limit = std::strtol(options["conn-reconnect-limit"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    int32_t conn_reconnect_timeout = -1;
    if (options.is_set("conn-reconnect-timeout")) {
        conn_reconnect_timeout = std::strtol(options["conn-reconnect-timeout"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    uint32_t conn_reconnect_first = 0;
    if (options.is_set("conn-reconnect-first")) {
        conn_reconnect_first = std::strtoul(options["conn-reconnect-first"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    uint32_t conn_reconnect_increment = 100;
    if (options.is_set("conn-reconnect-increment")) {
        conn_reconnect_increment = std::strtoul(options["conn-reconnect-increment"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    bool conn_reconnect_doubling = true;
    if (options.is_set("conn-reconnect-doubling")) {
        if (options["conn-reconnect-doubling"] == "false") {
            conn_reconnect_doubling = false;
        }

        conn_reconnect_custom = true;
    }

    uint32_t conn_heartbeat = 0;
    if (options.is_set("conn-heartbeat")) {
        conn_heartbeat = std::strtoul(options["conn-heartbeat"].c_str(), NULL, 10);
    }

    uint32_t max_frame_size = -1;
    if (options.is_set("conn-max-frame-size")) {
        max_frame_size = std::strtoul(options["conn-max-frame-size"].c_str(), NULL, 10);
    }

    string log_msgs = "";
    if (options.is_set("log-msgs")) {
        log_msgs = options["log-msgs"];
    }

    OptionsSetter setter = OptionsSetter(options);
    
    int timeout = 0;
    if (options.is_set("timeout")) {
        timeout = static_cast<int> (options.get("timeout"));
    }

    bool process_reply_to = false;
    if (options.is_set("process-reply-to")) {
        process_reply_to = options.get("process-reply-to");
    }

    bool browse = false;
    if (options.is_set("recv-browse")) {
        browse = options.get("recv-browse");
    }

    int count = 0;
    if (options.is_set("count")) {
        count = static_cast<int> (options.get("count"));
    }

    string selector = "";
    if (options.is_set("recv-selector")) {
        selector = options["recv-selector"];
    }

    ReceiverHandler handler = ReceiverHandler(
        address,
        msg_action,
        msg_action_size,
        user,
        password,
        sasl_mechanisms,
        timeout,
        count,
        conn_reconnect,
        conn_reconnect_interval,
        conn_reconnect_limit,
        conn_reconnect_timeout,
        conn_reconnect_first,
        conn_reconnect_increment,
        conn_reconnect_doubling,
        conn_reconnect_custom,
        conn_heartbeat,
        max_frame_size,
        log_msgs,
        process_reply_to,
        browse
    );

    if (selector != "") {
        handler.setSelector(selector);
    }

    default_container(handler).run();

    return 0;

}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
