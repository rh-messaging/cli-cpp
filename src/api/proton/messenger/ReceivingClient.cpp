/*
 * ReceivingClient.cpp
 *
 *  Created on: Apr 28, 2015
 *      Author: opiske
 */

#include "ReceivingClient.h"

namespace dtests {
namespace proton {
namespace messenger {

using dtests::proton::common::ModernClient;

using namespace std;

ReceivingClient::ReceivingClient()
	: super()
{
	// TODO Auto-generated constructor stub

}

ReceivingClient::~ReceivingClient() {
	// TODO Auto-generated destructor stub
}

void ReceivingClient::setMessengerOptions(const OptionsSetter &setter,
		Messenger& messenger) const {
	setter.setNumber("timeout", &messenger, &Messenger::setTimeout);
}

Message ReceivingClient::get(const Messenger& messenger) const {
	Message message = messenger.get();
	BOOST_LOG_TRIVIAL(info)<< "Correlation ID: " <<
	message.getCorrelationId();
	BOOST_LOG_TRIVIAL(info)<< "Message ID: " << message.getMessageId();
	BOOST_LOG_TRIVIAL(info)<< "Subject: " << message.getSubject();
	BOOST_LOG_TRIVIAL(debug)<< "Received data: " <<
	message.getBody().readString();
	return message;
}

int ReceivingClient::run(int argc, char** argv) const {
	const string usage = "usage: %prog [OPTION]... DIR [FILE]...";
	const string version = "1.0";
	const string desc = "C/C++ AMQ client on top of Proton-C";

	ReceiverOptionsParser parser = ReceiverOptionsParser();

	/**
	 * WARNING: do not reassign the result of chainned calls to usage/version/etc
	 *  because it causes the code to segfault. For example, this crashes:
	 *
	 *  ControlOptions parser = ControlOptions().usage(usage)
	 */

	parser.usage(usage).version(version).description(desc);

	optparse::Values options = parser.parse_args(argc, argv);

	parser.validate(options);

	const string address = options["broker-url"];
	Messenger messenger = Messenger(address);

	messenger.start();

	OptionsSetter setter = OptionsSetter(options);

	setMessengerOptions(setter, messenger);

	setLogLevel(options);

	int count = static_cast<int>(options.get("count"));


#ifdef ENABLE_BLOCKING
	bool blocking = static_cast<bool>(options.get("blocking"));
	messenger.setBlocking(blocking);
#endif // ENABLE_BLOCKING

	messenger.subscribe();

#ifdef ENABLE_BLOCKING
	int rxSize = Messenger::DEFAULT_RX_SIZE;

	if (options.is_set("tx-size")) {
		rxSize = static_cast<int>(options.get("tx-size"));
	}
	messenger.receive(rxSize);
#else
	messenger.receive();
#endif // ENABLE_BLOCKING

	if (count > 0) {
		for (int i = 0; i < count; i++) {
			BOOST_LOG_TRIVIAL(debug) << "Retrieving message " << i << " of " <<
					count;

			if (messenger.getIncomingCount() == 0) {
				if (count != i) {
					BOOST_LOG_TRIVIAL(warning) << "Received less messages than " <<
							"asked for";
					break;
				}
			}

			Message message = get(messenger);
		}
	}
	else {
		while(true) {
			if (messenger.getIncomingCount() > 0) {
				Message message = get(messenger);
			}
		}
	}

	if (options.is_set("close-sleep")) {
		int closeSleep = static_cast<int>(options.get("close-sleep"));

		BOOST_LOG_TRIVIAL(debug) << "Sleeping for " << closeSleep <<
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
