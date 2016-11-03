/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReceivingClient.cpp
 * Author: opiske
 * 
 * Created on October 19, 2015, 4:25 PM
 */

#include "ReceivingClient.h"

using proton::default_container;

namespace dtests {
namespace proton {
namespace reactor {

ReceivingClient::ReceivingClient()
    : super()
{
}

ReceivingClient::~ReceivingClient()
{
}

void ReceivingClient::setMessageOptions(const OptionsSetter &setter, message &msg) const
{

}

int ReceivingClient::run(int argc, char **argv) const
{
    const string usage = "usage: %prog [OPTION]... DIR [FILE]...";
    const string version = "1.0";
    const string desc = "C/C++ AMQ reactive API receiver client for Qpid Proton";

    ReceiverOptionsParser parser = ReceiverOptionsParser();

    /**
     * WARNING: do not reassign the result of chainned calls to usage/version/etc
     *  because it causes the code to segfault. For example, this crashes:
     *
     *  ControlOptions parser = ControlOptions().usage(usage)
     */
    parser.usage(usage).version(version).description(desc);

    optparse::Values options = parser.parse_args(argc, argv);

    parser.validate(options);

    setLogLevel(options);

    const string address = options["broker-url"];
    
    string user = options["user"];
  
    string password = options["password"];
  
    string sasl_mechanisms = options["sasl_mechanisms"];

    OptionsSetter setter = OptionsSetter(options);
    
    int timeout = 0;
    if (options.is_set("timeout")) {
        timeout = static_cast<int> (options.get("timeout"));
    }

    ReceiverHandler handler = ReceiverHandler(address, user, password, sasl_mechanisms, timeout);
    default_container(handler).run();

    return 0;

}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
