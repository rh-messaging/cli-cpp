/*
 * SendingClient.cpp
 *
 *  Created on: Apr 28, 2015
 *      Author: opiske
 */

#include "SendingClient.h"

namespace dtests {
namespace proton {
namespace messenger {

using namespace std;

using dtests::proton::common::ModernClient;
using namespace dtests::common::log;

namespace fs = boost::filesystem;

SendingClient::SendingClient()
	: super()
{

}

SendingClient::~SendingClient() {

}


void SendingClient::setMessageOptions(const OptionsSetter &setter,
		Message& message) const
{
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
}

void SendingClient::setMessengerOptions(const OptionsSetter &setter,
		Messenger& messenger) const
{
	setter.setNumber("timeout", &messenger, &Messenger::setTimeout);

#ifdef ENABLE_BLOCKING
	setter.setBoolean("blocking", &messenger, &Message::setBlocking);
#endif // ENABLE_BLOCKING
}





int SendingClient::run(int argc, char** argv) const {
	const string usage = "usage: %prog [OPTION]... DIR [FILE]...";
	const string version = "1.0";
	const string desc = "C/C++ AMQ client on top of Proton-C";

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
	Messenger messenger = Messenger(address);

	messenger.start();

	Message message = messenger.newMessage();

	OptionsSetter setter = OptionsSetter(options);

	setMessageOptions(setter, message);

	const string content = setter.getContent();
	message.setBody(content);


	setMessengerOptions(setter, messenger);

	int count = static_cast<int>(options.get("count"));
	for (int i = 0; i < count; i++) {
		messenger.put(message);

		logger(info) << "Correlation ID: " <<
				message.getCorrelationId();

		logger(info) << "Message ID: " << message.getMessageId();

		logger(debug) << "Link capacity: " <<
				messenger.getCapacity();
	}

#ifdef ENABLE_BLOCKING
	if (options.is_set("close-sleep")) {
		logger(info) << "Sleeping before sending";
		int closeSleep = static_cast<int>(options.get("close-sleep"));
		usleep(1000 * closeSleep);
	}
#endif // ENABLE_BLOCKING



#ifdef ENABLE_BLOCKING
	int txSize = Messenger::DEFAULT_TX_SIZE;

	txSize = static_cast<int>(options.get("tx-size"));
	messenger.send(txSize);
#else
	messenger.send();
#endif // ENABLE_BLOCKING

	logger(info) << "Sent!";

	if (options.is_set("close-sleep")) {
		int closeSleep = static_cast<int>(options.get("close-sleep"));

		logger(debug) << "Sleeping for " << closeSleep <<
					" milliseconds before closing the connection";

		// TODO: duplicated stuff -> Check Utils.msleep
		usleep(1000 * closeSleep);
	}

	messenger.stop();

	return 0;
}

} /* namespace messenger */
} /* namespace proton */
} /* namespace dtests */
