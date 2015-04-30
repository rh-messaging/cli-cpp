/*
 * Messenger.cpp
 *
 *  Created on: Apr 15, 2015
 *      Author: opiske
 */

#include "Messenger.h"

namespace dtests {
namespace proton {
namespace messenger {

using namespace dtests::common;

Messenger::Messenger(const string& address)
	: super(),
	  messenger(NULL),
	  address(address)
{
	init();
}

Messenger::~Messenger() {
	pn_messenger_free(messenger);
}

void Messenger::init() {
	BOOST_LOG_TRIVIAL(info) << "Initializing messenger";
	messenger = pn_messenger(NULL);

	if (messenger == NULL) {
		throw ProtonException("Unable to create a new messenger object");
	}
}

void Messenger::start() {
	BOOST_LOG_TRIVIAL(info) << "Starting messenger";

	pn_messenger_start(messenger);
}


void Messenger::stop() {
	BOOST_LOG_TRIVIAL(info) << "Stopping messenger";

	pn_messenger_stop(messenger);
}

void Messenger::setCapacity(int capacity) {
	// TODO: apparently this is not supported.
}

int Messenger::getCapacity() const {
	pn_link_t *link = pn_messenger_get_link(messenger, address.c_str(), true);

	return pn_link_credit(link);
}


bool Messenger::failed() const {
	if (pn_messenger_errno(messenger)) {
		return true;
	}

	return false;
}


void Messenger::handleError() const {
	pn_error_t *error = pn_messenger_error(messenger);

	throw ProtonError::handleError<ProtonException>(error, "");

}

void Messenger::setBlocking(bool block) {
	pn_messenger_set_blocking(messenger, block);
}

bool Messenger::isBlocking() const {
	return pn_messenger_is_blocking(messenger);
}


void Messenger::put(const char* data, size_t size) const {
	BOOST_LOG_TRIVIAL(debug) << "Putting " << size << " bytes on the outgoing "
			<< "queue: " << data;

	if (isBlocking()) {
		BOOST_LOG_TRIVIAL(debug) << "The messenger is in blocking mode";
	}

	pn_message_t *message = pn_message();

	pn_message_set_address(message, address.c_str());

	pn_data_t *body = pn_message_body(message);
	pn_data_put_string(body, pn_bytes(size, data));

	pn_messenger_put(messenger, message);
	if (failed()) {
		BOOST_LOG_TRIVIAL(error) << "Failed to put message in the messenger";
		pn_error_t *error = pn_messenger_error(messenger);

		throw ProtonError::handleError<IOException>(error,
				"Failed to put message in the messenger");
	}

	BOOST_LOG_TRIVIAL(trace) << "Successfully put the data";
}

void Messenger::put(const Message& message) const {
	if (isBlocking()) {
		BOOST_LOG_TRIVIAL(debug) << "The messenger is in blocking mode";
	}

	pn_message_t *data = message.getMessage();

	pn_messenger_put(messenger, data);
	if (failed()) {
		BOOST_LOG_TRIVIAL(error) << "Failed to put message in the messenger";
		pn_error_t *error = pn_messenger_error(messenger);

		throw ProtonError::handleError<IOException>(error,
				"Failed to put message in the messenger");
	}

	BOOST_LOG_TRIVIAL(trace) << "Successfully put the data";
}


void Messenger::send(int messageCount) const {
	if (isBlocking()) {
		BOOST_LOG_TRIVIAL(debug) << "The messenger is in blocking mode";
	}

	BOOST_LOG_TRIVIAL(debug) << "Sending " << messageCount << " messages";

	pn_messenger_send(messenger, messageCount);
	if (failed()) {
		BOOST_LOG_TRIVIAL(error) << "Failed to send message";
		pn_error_t *error = pn_messenger_error(messenger);

		throw ProtonError::handleError<IOException>(error,
				"Failed to send message");
	}

	BOOST_LOG_TRIVIAL(trace) << "Successfully sent the messages";
}

Message Messenger::newMessage() const {
	Message message = Message(address);

	return message;
}

const string& Messenger::getAddress() const {
	return address;
}

void Messenger::setTimeout(int timeout) {
	pn_messenger_set_timeout(messenger, timeout);
}

int Messenger::getTimeout() const {
	return pn_messenger_get_timeout(messenger);
}

void Messenger::receive(int count) const {
	pn_messenger_recv(messenger, count);

	if (failed()) {
		BOOST_LOG_TRIVIAL(error) << "Failed to receive message";
		pn_error_t *error = pn_messenger_error(messenger);

		throw ProtonError::handleError<IOException>(error,
				"Failed to receive message");
	}
}


Message Messenger::get() const {
	Message ret = newMessage();

	pn_message_t *message = ret.getMessage();
	pn_messenger_get(messenger, message);
	if (failed()) {
		BOOST_LOG_TRIVIAL(error) << "Failed to receive messages from the " <<
				"incoming queue";
		pn_error_t *error = pn_messenger_error(messenger);

		throw ProtonError::handleError<IOException>(error,
				"Failed to receive messages from the incoming queue");
	}

	return ret;
}


int Messenger::getIncomingCount() const {
	return pn_messenger_incoming(messenger);
}

void Messenger::subscribe() const {
	pn_messenger_subscribe(messenger, address.c_str());
	if (failed()) {
		BOOST_LOG_TRIVIAL(error) << "Failed to subscribe to the receiving " <<
				"address " << address;
		pn_error_t *error = pn_messenger_error(messenger);

		throw ProtonError::handleError<IOException>(error,
				"Failed to subscribe to the receiving address");
	}
}

}}}
