/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TxReceivingClient.h
 * Author: pematous
 *
 * Created on October 19, 2015, 4:25 PM
 */

#ifndef TXRECEIVINGCLIENT_H
#define TXRECEIVINGCLIENT_H

#include <sstream>
#include <string>

#include <proton/message.hpp>
#include <proton/container.hpp>
#include <proton/uuid.hpp>

#include "ReceivingClient.h"
#include "handler/TxReceiverHandler.h"
#include "options/modern/ReceiverOptionsParser.h"
#include "options/OptionsHelper.h"
#include "TypeUtils.h"
#include "UriParser.h"

using proton::message;
using proton::uuid;

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;

/**
 * Implements a receiving client using the proton reactive API
 */
class TxReceivingClient : public ReceivingClient {
  public:
    TxReceivingClient();
    virtual ~TxReceivingClient();

    virtual int run(int argc, char **argv) const;

  private:
    typedef ReceivingClient super;
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* TXRECEIVINGCLIENT_H */

