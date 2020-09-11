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
	
	// setter.set("msg-correlation-id", &msg, 
	//	static_cast<reactor_setter>(&message::correlation_id));
	
	// setter.set("msg-content-type", &message, 
	//	static_cast<string_setter>(&message::content_type));
	
	/*
	
	 */
/*
	setter.set("msg-id", &message, &Message::setMessageId);
	setter.set("msg-user-id", &message, &Message::setUserId);
	setter.set("msg-subject", &message, &Message::setSubject);
	
	setter.setNumber("msg-ttl", &message, &Message::setTimeToLive);
	setter.setNumber("msg-priority", &message, &Message::setPriority);
	setter.setBoolean("msg-durable", &message, &Message::setDurable);
	setter.setMap("msg-properties", &message, &Message::setProperties);
*/	
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

	//message msg = message();

	//msg.body(content);

	SenderHandler handler = SenderHandler(address);
	container(handler).run();
	
	return 0;

}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */