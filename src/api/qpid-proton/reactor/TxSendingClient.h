/*
 * TxSendingClient.h
 *
 *  Created on: Nov 20, 2024
 *      Author: pematous
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_TXSENDINGCLIENT_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_TXSENDINGCLIENT_H_

#include <sstream>
#include <string>

#include <proton/message.hpp>
#include <proton/container.hpp>
#include <proton/types.hpp>
#include <proton/duration.hpp>

#include "SendingClient.h"
#include "handler/TxSenderHandler.h"
#include "options/modern/SenderOptionsParser.h"
#include "options/OptionsHelper.h"
#include "ContentType.h"
#include "TypeUtils.h"
#include "UriParser.h"

using proton::message;

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;

/**
 * Implements a connecting client using the proton reactive API
 */
class TxSendingClient : public SendingClient {
  public:
    TxSendingClient();
    virtual ~TxSendingClient();

    virtual int run(int argc, char **argv) const;

  private:
    typedef SendingClient super;
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_REACTOR_TXSENDINGCLIENT_H_ */
