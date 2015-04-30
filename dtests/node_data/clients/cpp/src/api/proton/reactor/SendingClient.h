/*
 * SendingClient.h
 *
 *  Created on: Apr 30, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_

#include <optparse/OptionParser.h>

#include <boost/log/trivial.hpp>

#include <proton/cpp/Container.h>

#include "ModernClient.h"
#include "SimpleMessagingHandler.h"
#include "options/modern/SenderOptionsParser.h"
#include "options/OptionsHelper.h"

using proton::reactor::Message;

namespace dtests {
namespace proton {
namespace reactor {

using dtests::proton::common::ModernClient;

class SendingClient: public ModernClient {
public:
	SendingClient();
	virtual ~SendingClient();

	virtual int run(int argc, char **argv) const;

private:
	typedef ModernClient super;

	void setMessageOptions(const OptionsSetter &setter, Message& message) const;
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_ */
