/*
 * SendingClient.h
 *
 *  Created on: Apr 30, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_


#include <proton/message.hpp>
#include <proton/default_container.hpp>
#include <proton/types.hpp>

#include "ModernClient.h"
#include "handler/SenderHandler.h"
#include "options/modern/SenderOptionsParser.h"
#include "options/OptionsHelper.h"
#include "Normalizer.h"
#include "ContentType.h"
#include "TypeUtils.h"
#include "UriParser.h"
#include "reactor/formatter/ReactorDecoder.h"
#include "formatter/DictFormatter.h"
#include "formatter/DictWriter.h"

using proton::message;

namespace dtests {
namespace proton {
namespace reactor {

using dtests::proton::common::ModernClient;

using namespace dtests::common;

/**
 * Implements a connecting client using the proton reactive API
 */
class SendingClient : public ModernClient {
  public:
    SendingClient();
    virtual ~SendingClient();

    virtual int run(int argc, char **argv) const;

  private:
    typedef ModernClient super;

    void setMessageOptions(const OptionsSetter &setter, message &msg) const;
    void setMessageContent(const OptionsSetter &setter, 
        const optparse::Values &options, message *msg) const;
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_SENDINGCLIENT_H_ */
