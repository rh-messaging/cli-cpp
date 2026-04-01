/*
 * TxSendingClient.cpp
 *
 *  Created on: Nov 20, 2024
 *      Author: pematous
 */

#include "TxSendingClient.h"

using proton::message;
using proton::container;


namespace dtests {
namespace proton {
namespace reactor {

using namespace std;
using dtests::common::UriParser;

TxSendingClient::TxSendingClient()
: super()

{

}

TxSendingClient::~TxSendingClient()
{

}

int TxSendingClient::run(int argc, char **argv) const
{
    const string usage = "usage: %prog [OPTION]... DIR [FILE]...";
    const string version = "1.0";
    const string desc = "C/C++ AMQ reactive API sender client for Qpid Proton";

    SenderOptionsParser parser = SenderOptionsParser();
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

    std::vector<std::string> conn_urls;
    if (options.is_set("conn-urls")) {
        std::stringstream conn_urls_string(options["conn-urls"]);
        std::string segment;

        while(std::getline(conn_urls_string, segment, ',')) {
           conn_urls.push_back(segment);
        }
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

    string conn_sasl_enabled = "true";
    if (options.is_set("conn-sasl-enabled")) {
        conn_sasl_enabled = options["conn-sasl-enabled"];
        std::transform(conn_sasl_enabled.begin(), conn_sasl_enabled.end(), conn_sasl_enabled.begin(), ::tolower);
    }

    string conn_ssl_certificate = "";
    if (options.is_set("conn-ssl-certificate")) {
        conn_ssl_certificate = options["conn-ssl-certificate"];
    }

    string conn_ssl_private_key = "";
    if (options.is_set("conn-ssl-private-key")) {
        conn_ssl_private_key = options["conn-ssl-private-key"];
    }

    string conn_ssl_password = "";
    if (options.is_set("conn-ssl-password")) {
        conn_ssl_password = options["conn-ssl-password"];
    }

    string conn_ssl_trust_store = "";
    if (options.is_set("conn-ssl-trust-store")) {
        conn_ssl_trust_store = options["conn-ssl-trust-store"];
    }

    bool conn_ssl_verify_peer = options.is_set("conn-ssl-verify-peer");

    bool conn_ssl_verify_peer_name = options.is_set("conn-ssl-verify-peer-name");

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

    OptionsSetter setter = OptionsSetter(options);

    int timeout = 1;
    if (options.is_set("timeout")) {
        timeout = static_cast<int> (options.get("timeout"));
    }

    string log_msgs = "";
    if (options.is_set("log-msgs")) {
        log_msgs = options["log-msgs"];
    }

    int duration = 0;
    if (options.is_set("duration")) {
        duration = static_cast<int> (options.get("duration"));
    }

    string duration_mode = "after-send";
    if (options.is_set("duration-mode")) {
        duration_mode = options["duration-mode"];
    }

    bool trace_messages = false;
    if (options.is_set("trace-messages")) {
        if (options["trace-messages"] == "1" || options["trace-messages"] == "true") {
            trace_messages = true;
        }
    }
    if (trace_messages) {
        enableTracing("aac3_sender");
    }

    string tx_action = "commit";
    if (options.is_set("tx-action")) {
        tx_action = options["tx-action"];
    }

    string tx_endloop_action = "none";
    if (options.is_set("tx-endloop-action")) {
        tx_endloop_action = options["tx-endloop-action"];
    }

    message msg;

    setMessageOptions(setter, msg);
    setMessageProperties(parser.callbackProperty, &msg);

    if (parser.callbackList.str.length() > 0) {
        // List
        setMessageList(parser.callbackList, &msg);
    } else if (parser.callbackMap.str.length() > 0) {
        // Map
        setMessageMap(parser.callbackMap, &msg);
    } else {
        // Text
        setMessageText(options["msg-content"], &msg);
    }


/*
 * Note 1: this is a left-over from setMessageOptions. Since I don't want to 
 * change the method signature there, I check again here and set the remaining
 * option that cannot be done implicitly above.
 *
 * Note 2: this is a hack for GCC ~4.4.7 on i686. 
 */
#ifndef ENABLE_IMPLICIT_CONVERSIONS
    long value =  options.get("msg-ttl");

    msg.ttl(::proton::duration(value));
#endif
    if (options.is_set("msg-group-seq")) {
        msg.group_sequence(std::strtol(options["msg-group-seq"].c_str(), NULL, 10));
    }

    bool conn_use_config_file = options.is_set("conn-use-config-file");

    TxSenderHandler handler = TxSenderHandler(
        address,
        conn_urls,
        is_topic,
        user,
        password,
        sasl_mechanisms,
        conn_sasl_enabled,
        conn_ssl_certificate,
        conn_ssl_private_key,
        conn_ssl_password,
        conn_ssl_trust_store,
        conn_ssl_verify_peer,
        conn_ssl_verify_peer_name,
        timeout,
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
        conn_use_config_file,
        log_msgs,
        tx_action,
        tx_endloop_action
    );

    handler.setMessage(msg);

    int count = 1;
    if (options.is_set("count")) {
        count = static_cast<int> (options.get("count"));
    }
    handler.setCount(count);

    int tx_size = 0;
    if (options.is_set("tx-size")) {
        tx_size = static_cast<int> (options.get("tx-size"));
    }
    handler.setBatchSize(tx_size);

    try {
        container(handler).run();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }

    return 1;
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
