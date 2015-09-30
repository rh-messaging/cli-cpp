/*
 * SendingClient.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: opiske
 */

#include "SendingClient.h"

#ifdef REACTOR_ENABLED

using proton::reactor::Message;
using proton::reactor::Container;

namespace dtests {
namespace proton {
namespace reactor {

using std::string;
using dtests::proton::common::ModernClient;

SendingClient::SendingClient()
	: super()
{

}

SendingClient::~SendingClient() {

}

void SendingClient::setMessageOptions(const OptionsSetter &setter,
		Message& message) const
{
	/*
	setter.set("msg-reply-to", &message, &Message::setReplyTo);
	setter.set("msg-correlation-id", &message, &Message::setCorrelationId);
	setter.set("msg-id", &message, &Message::setMessageId);
	setter.set("msg-user-id", &message, &Message::setUserId);
	setter.set("msg-subject", &message, &Message::setSubject);
	setter.set("msg-content-type", &message, &Message::setContentType);
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

	Message message = Message();

	message.setBody(content);

	SimpleMessagingHandler handler = SimpleMessagingHandler(address);
	Container(handler).run();
	
	return 0;

}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif // REACTOR_ENABLED