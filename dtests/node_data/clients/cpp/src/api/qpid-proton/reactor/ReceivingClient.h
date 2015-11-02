/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReceivingClient.h
 * Author: opiske
 *
 * Created on October 19, 2015, 4:25 PM
 */

#ifndef RECEIVINGCLIENT_H
#define RECEIVINGCLIENT_H

#include <proton/message.hpp>
#include <proton/container.hpp>

#include "ModernClient.h"
#include "handler/ReceiverHandler.h"
#include "options/modern/ReceiverOptionsParser.h"
#include "options/OptionsHelper.h"
#include "TypeUtils.h"

using proton::message;

namespace dtests {
namespace proton {
namespace reactor {

using dtests::proton::common::ModernClient;

using namespace dtests::common;

/**
 * Implements a receiving client using the proton reactive API
 */
class ReceivingClient : public ModernClient {
  public:
    ReceivingClient();
    virtual ~ReceivingClient();

    virtual int run(int argc, char **argv) const;

  private:
    typedef ModernClient super;

    void setMessageOptions(const OptionsSetter &setter, message &msg) const;

};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* RECEIVINGCLIENT_H */

