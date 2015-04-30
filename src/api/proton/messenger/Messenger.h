/*
 * Messenger.h
 *
 *  Created on: Apr 15, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_MESSENGER_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_MESSENGER_H_

#include <proton/messenger.h>
#include <boost/log/trivial.hpp>

#include <string>

#include "ProtonObject.h"
#include "Message.h"
#include "ProtonError.h"
#include "IOException.h"

namespace dtests {
namespace proton {
namespace messenger {

using namespace std;

class Messenger: public ProtonObject {

public:
	/**
	 * The default transaction size will cause the messenger to send all the
	 * messages in the outgoing queue
	 */
	const static int DEFAULT_TX_SIZE = -1;

	/**
	 * The default receive size will cause the messenger to receive all the
	 * messages in the incoming queue
	 */
	const static int DEFAULT_RX_SIZE = -1;

	Messenger(const string &address);
	virtual ~Messenger();

	void start();
	void stop();

	Message newMessage() const;

	void setTimeout(int timeout);
	int getTimeout() const;

	void setCapacity(int capacity);
	int getCapacity() const;

	void setBlocking(bool block);
	bool isBlocking() const;

	void put(const char *data, size_t size) const;
	void put(const Message &message) const;

	void send(int messageCount = DEFAULT_TX_SIZE) const;
	const string& getAddress() const;

	void subscribe() const;
	void receive(int count = DEFAULT_RX_SIZE) const;
	int getIncomingCount() const;
	Message get() const;

private:
	typedef ProtonObject super;

	pn_messenger_t *messenger;
	const string address;

	void init();

	bool failed() const;
	void handleError() const;
};

}}}

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_MESSENGER_H_ */
