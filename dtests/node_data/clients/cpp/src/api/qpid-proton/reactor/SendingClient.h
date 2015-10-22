/*
 * SendingClient.h
 *
 *  Created on: Apr 30, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_


#include <proton/message.hpp>
#include <proton/container.hpp>

#include "ModernClient.h"
#include "handler/SenderHandler.h"
#include "options/modern/SenderOptionsParser.h"
#include "options/OptionsHelper.h"
#include "Normalizer.h"

using proton::message;

namespace dtests {
namespace proton {
namespace reactor {
    
typedef void (message::*string_setter)(const string &);
typedef void (message::*data_setter)(const data &);

using dtests::proton::common::ModernClient;

class SendingClient: public ModernClient {
public:
	SendingClient();
	virtual ~SendingClient();

	virtual int run(int argc, char **argv) const;

private:
	typedef ModernClient super;

	void setMessageOptions(const OptionsSetter &setter, message &msg) const;
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_ */
