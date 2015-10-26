/*
 * ReceivingClient.h
 *
 *  Created on: Apr 28, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_RECEIVINGCLIENT_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_RECEIVINGCLIENT_H_

#include <optparse/OptionParser.h>

#include "Message.h"
#include "Messenger.h"

#include "ModernClient.h"
#include "options/modern/ReceiverOptionsParser.h"
#include "options/OptionsHelper.h"

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

namespace dtests {
namespace proton {
namespace messenger {

using dtests::proton::common::ModernClient;

class ReceivingClient : public ModernClient {
  public:
    ReceivingClient();
    virtual ~ReceivingClient();

    virtual int run(int argc, char **argv) const;

  private:
    typedef ModernClient super;

    void setMessageOptions(const OptionsSetter &setter, Message& message) const;
    void setMessengerOptions(const OptionsSetter &setter, Messenger& messenger) const;
    Message get(const Messenger& messenger) const;
};

} /* namespace messenger */
} /* namespace proton */
} /* namespace dtests */

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_RECEIVINGCLIENT_H_ */
