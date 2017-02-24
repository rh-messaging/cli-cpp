/*
 * SendingClient.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: opiske
 */

#include <proton/message.hpp>
#include "SendingClient.h"

using proton::message;
using proton::default_container;


namespace dtests {
namespace proton {
namespace reactor {

using namespace std;
using dtests::common::UriParser;
using dtests::proton::common::ModernClient;

SendingClient::SendingClient()
: super()

{

}

SendingClient::~SendingClient()
{

}

void SendingClient::setMessageOptions(const OptionsSetter &setter,
        message &msg) const
{
    /*
     * Why the cast, you might ask? Because reply_to is overloaded, 
     * and the compiler is not smart enough to pick the correct one. 
     * Therefore we cast it (but not really) so that the compiler knows 
     * we are talking about reactor_type member function pointer (which is
     * defined in the header file analogous to this one.
     */
    setter.set("msg-reply-to", &msg,
            static_cast<string_setter> (&message::reply_to));

    setter.set("msg-correlation-id", &msg,
            static_cast<id_setter> (&message::correlation_id),
            &idNormalizer);

    setter.set("msg-content-type", &msg,
            static_cast<string_setter> (&message::content_type));

    setter.set("msg-subject", &msg,
            static_cast<string_setter> (&message::subject));

    setter.set("msg-id", &msg, static_cast<id_setter> (&message::id),
            &idNormalizer);

#ifdef ENABLE_IMPLICIT_CONVERSIONS
    timestamp def = timestamp(-1);
    setter.setNumber("msg-ttl", &msg,
            static_cast<timestamp_setter> (&message::expiry_time), def);
#endif

    setter.set("msg-user-id", &msg,
            static_cast<string_setter> (&message::user));

    setter.setNumber("msg-priority", &msg,
            static_cast<uint_setter> (&message::priority));

    setter.setBoolean("msg-durable", &msg,
            static_cast<boolean_setter> (&message::durable));

#ifdef REACTOR_PROPERTY_MAP_USES_STL
    message::property_map &properties = msg.properties();

    setter.setMap("msg-properties", properties);
#endif // REACTOR_PROPERTY_MAP_USES_STL
}

void SendingClient::setMessageContent(const OptionsSetter &setter,
        const optparse::Values &options, message *msg) const
{
    string content_type = options["msg-content-type"];

    const string content = setter.getContent();

    if (content_type == ContentType::AMQP_LIST) {
        setter.setList("msg-content", msg,
                static_cast<value_setter> (&message::body), &listNormalizer);

    } else {
        if (content_type == ContentType::AMQP_MAP) {
            setter.setMap("msg-content", msg,
                    static_cast<value_setter> (&message::body), &mapNormalizer);

        } else {
            std::cout << "Sending: " << content << std::endl;
            msg->body(content);
        }
    }
}

int SendingClient::run(int argc, char **argv) const
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

    const string address = options["broker-url"];
    
    uri_parser.parse(options["broker-url"]);

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
    } else if (user != "" || password != "") {
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

    uint32_t max_frame_size = -1;
    if (options.is_set("conn-max-frame-size")) {
        max_frame_size = std::strtoul(options["conn-max-frame-size"].c_str(), NULL, 10);
    }

    OptionsSetter setter = OptionsSetter(options);

    int timeout = 0;
    if (options.is_set("timeout")) {
        timeout = static_cast<int> (options.get("timeout"));
    }

    message msg;

    setMessageOptions(setter, msg);
    setMessageContent(setter, options, &msg);
   
/*
 * Note 1: this is a left-over from setMessageOptions. Since I don't want to 
 * change the method signature there, I check again here and set the remaining
 * option that cannot be done implicitly above.
 * 
 * Note 2: this is a hack for GCC ~4.4.7 on i686. 
 */    
#ifndef ENABLE_IMPLICIT_CONVERSIONS
    long value =  options.get("msg-ttl");
    
    msg.expiry_time(timestamp(value));
#endif

    SenderHandler handler = SenderHandler(
        address,
        user,
        password,
        sasl_mechanisms,
        timeout,
        conn_reconnect,
        conn_reconnect_interval,
        conn_reconnect_limit,
        conn_reconnect_timeout,
        conn_reconnect_first,
        conn_reconnect_increment,
        conn_reconnect_doubling,
        conn_reconnect_custom,
        max_frame_size
    );

    handler.setMessage(msg);
    
    int count = 1;
    if (options.is_set("count")) {
        count = static_cast<int> (options.get("count"));
    }
    handler.setCount(count);
    
    default_container(handler).run();

    return 0;

}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
