/*
 * SendingClient.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: opiske
 */

#include <proton/message.hpp>
#include "SendingClient.h"


using proton::message;
using proton::container;


namespace dtests {
namespace proton {
namespace reactor {

using namespace std;
using dtests::proton::common::ModernClient;

SendingClient::SendingClient()
	: super()
        
{

}

SendingClient::~SendingClient() {

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
		static_cast<string_setter>(&message::reply_to));
	
	setter.set("msg-correlation-id", &msg, 
		static_cast<data_setter>(&message::correlation_id), 
                &defaultNormalizer);
	
	setter.set("msg-content-type", &msg,
                static_cast<string_setter>(&message::content_type));
	
	setter.set("msg-subject", &msg, 
                static_cast<string_setter>(&message::subject));
        
        setter.set("msg-id", &msg, static_cast<data_setter>(&message::id), 
                &defaultNormalizer);
        
        setter.setNumber("msg-ttl", &msg, 
                static_cast<amqpts_setter>(&message::expiry_time));
        
        setter.set("msg-user-id", &msg,
                static_cast<string_setter>(&message::user));
        
        // TODO: not implemented on reactor
        // setter.setNumber("msg-priority", &msg, static_cast<long_setter>(&message::priority));
        // setter.setBoolean("msg-durable", &msg, static_cast<boolean_setter>(&message::durable));
        // setter.setMap("msg-properties", &msg, static_cast<map_setter>(&message::properties));
}
 


int SendingClient::run(int argc, char **argv) const {
	const string usage = "usage: %prog [OPTION]... DIR [FILE]...";
	const string version = "1.0";
	const string desc = "C/C++ AMQ reactor client for Proton-C";

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
	const string content = setter.getContent();

	message msg = message();

        setMessageOptions(setter, msg);
	msg.body(content);

	SenderHandler handler = SenderHandler(address);
        
        handler.setMessage(msg);
	container(handler).run();
	
	return 0;

}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */