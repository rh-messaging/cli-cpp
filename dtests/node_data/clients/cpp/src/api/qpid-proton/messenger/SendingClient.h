/*
 * SendingClient.h
 *
 *  Created on: Apr 28, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_SENDINGCLIENT_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_SENDINGCLIENT_H_

#include <optparse/OptionParser.h>

#include <vector>
#include <ctime>

#include "Message.h"
#include "Messenger.h"

#include "ModernClient.h"

#include "options/modern/SenderOptionsParser.h"
#include "options/OptionsHelper.h"

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

namespace dtests {
namespace proton {
namespace messenger {

using dtests::proton::common::ModernClient;

class SendingClient: public ModernClient {
public:
	SendingClient();
	virtual ~SendingClient();

	virtual int run(int argc, char **argv) const;

private:
	typedef ModernClient super;

	void setMessageOptions(const OptionsSetter &setter, Message& message) const;
	void setMessengerOptions(const OptionsSetter &setter, Messenger& messenger) const;

};

} /* namespace messenger */
} /* namespace proton */
} /* namespace dtests */

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_SENDINGCLIENT_H_ */
