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

    timestamp def = timestamp(-1);
    setter.setNumber("msg-ttl", &msg,
            static_cast<timestamp_setter> (&message::expiry_time), def);

    setter.set("msg-user-id", &msg,
            static_cast<string_setter> (&message::user));

    setter.setNumber("msg-priority", &msg,
            static_cast<uint_setter> (&message::priority));

    setter.setBoolean("msg-durable", &msg,
            static_cast<boolean_setter> (&message::durable));

    message::property_map &properties = msg.properties();

    setter.setMap("msg-properties", properties);
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

    OptionsSetter setter = OptionsSetter(options);

    message msg;

    setMessageOptions(setter, msg);
    setMessageContent(setter, options, &msg);

    SenderHandler handler = SenderHandler(address);

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