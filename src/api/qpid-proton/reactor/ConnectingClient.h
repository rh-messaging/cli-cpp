/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConnectingClient.h
 * Author: opiske
 *
 * Created on October 14, 2015, 1:24 PM
 */

#ifndef CONNECTINGCLIENT_H
#define CONNECTINGCLIENT_H

#include <proton/container.hpp>

#include "ModernClient.h"
#include "handler/ConnectorHandler.h"
#include "options/modern/ConnectorOptionsParser.h"
#include "options/OptionsHelper.h"

namespace dtests {
namespace proton {
namespace reactor {

using dtests::proton::common::ModernClient;
using namespace dtests::common;

/**
 * Implements a connecting client using the proton reactive API
 */
class ConnectingClient : public ModernClient {
  public:
    ConnectingClient();
    virtual ~ConnectingClient();

    virtual int run(int argc, char **argv) const;

  private:
    typedef ModernClient super;

    void setMessageHandlerOptions(const OptionsSetter &setter,
            ConnectorHandler &handler) const;
    
    int setConnectionOptions(const optparse::Values &options) const;

};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* CONNECTINGCLIENT_H */

