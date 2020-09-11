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

using proton::container;

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

    // Temporary variable for address, will search for prefix
    string tempAddress = options["broker-url"];
    // Variable for final address
    string address;
    // Variable for recognition of topic
    bool is_topic = false;
    // Search for prefix
    std::size_t prefix_index = tempAddress.find("topic://");
    // If prefix is present
    if (prefix_index != std::string::npos) {
        // Delete prefix
        address = tempAddress.replace(prefix_index, 8, "");
        // Set that it will be topic
        is_topic = true;
    // If prefix is NOT present
    } else {
        // Use full address
        address = tempAddress;
    }

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

    bool durable_subscriber = false;
    if (options.is_set("durable-subscriber")) {
        string durable_subscriber_lower = options["durable-subscriber"];
        std::transform(durable_subscriber_lower.begin(), durable_subscriber_lower.end(), durable_subscriber_lower.begin(), ::tolower);

        if (durable_subscriber_lower == "true") {
            durable_subscriber = true;
        }
    }

    bool subscriber_unsubscribe = false;
    if (options.is_set("subscriber-unsubscribe")) {
        string subscriber_unsubscribe_lower = options["subscriber-unsubscribe"];
        std::transform(subscriber_unsubscribe_lower.begin(), subscriber_unsubscribe_lower.end(), subscriber_unsubscribe_lower.begin(), ::tolower);

        if (subscriber_unsubscribe_lower == "true") {
            subscriber_unsubscribe = true;
        }
    }

    string durable_subscriber_prefix = "";
    if (options.is_set("durable-subscriber-prefix")) {
        durable_subscriber_prefix = options["durable-subscriber-prefix"];
    }

    string durable_subscriber_name = "";
    if (options.is_set("durable-subscriber-name")) {
        durable_subscriber_name = options["durable-subscriber-name"];
    }

    bool shared_subscriber = false;
    if (options.is_set("shared-subscriber")) {
        string shared_subscriber_lower = options["shared-subscriber"];
        std::transform(shared_subscriber_lower.begin(), shared_subscriber_lower.end(), shared_subscriber_lower.begin(), ::tolower);

        if (shared_subscriber_lower == "true") {
            shared_subscriber = true;
        }
    }

    string conn_clientid = "";
    if (options.is_set("conn-clientid")) {
        conn_clientid = options["conn-clientid"];
    }

    string conn_clientid_prefix = "";
    if (options.is_set("conn-clientid-prefix")) {
        conn_clientid_prefix = options["conn-clientid-prefix"];
    }

    bool conn_reconnect_custom = false;

    string conn_reconnect = "true";
    if (options.is_set("conn-reconnect")) {
        conn_reconnect = options["conn-reconnect"];
        std::transform(conn_reconnect.begin(), conn_reconnect.end(), conn_reconnect.begin(), ::tolower);
    }

    int32_t conn_reconnect_interval = 10;
    if (options.is_set("conn-reconnect-interval")) {
        conn_reconnect_interval = std::strtol(options["conn-reconnect-interval"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    int32_t conn_reconnect_limit = 0;
    if (options.is_set("conn-reconnect-limit")) {
        conn_reconnect_limit = std::strtol(options["conn-reconnect-limit"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    int32_t conn_reconnect_timeout = duration::FOREVER.milliseconds();
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
    
    int timeout = -1;
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

    int duration = 0;
    if (options.is_set("duration")) {
        duration = static_cast<int> (options.get("duration"));
    }

    string duration_mode = "after-receive";
    if (options.is_set("duration-mode")) {
        duration_mode = options["duration-mode"];
    }

    int recv_listen_port = 5672;
    if(options.is_set("recv-listen-port")) {
        recv_listen_port = atoi(options["recv-listen-port"].c_str());
    }

    string recv_listen = "false";
    if (options.is_set("recv-listen")) {
        recv_listen = options["recv-listen"];
        std::transform(recv_listen.begin(), recv_listen.end(), recv_listen.begin(), ::tolower);
    }

    ReceiverHandler handler = ReceiverHandler(
        address,
        is_topic,
        durable_subscriber,
        subscriber_unsubscribe,
        durable_subscriber_prefix,
        durable_subscriber_name,
        shared_subscriber,
        msg_action,
        msg_action_size,
        user,
        password,
        sasl_mechanisms,
        timeout,
        count,
        duration,
        duration_mode,
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
        browse,
        recv_listen,
        recv_listen_port
    );

    if (selector != "") {
        handler.setSelector(selector);
    }

    try {
        if (conn_clientid != "") {
            container(handler, conn_clientid).run();
        } else if (conn_clientid_prefix != "") {
            conn_clientid = conn_clientid_prefix + ::proton::uuid::random().str();

            container(handler, conn_clientid).run();
        } else {
            container(handler).run();
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }

    return 1;
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
