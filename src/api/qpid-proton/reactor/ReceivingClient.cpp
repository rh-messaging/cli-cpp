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

using proton::container;

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

    OptionsSetter setter = OptionsSetter(options);

    ReceiverHandler handler = ReceiverHandler(address);
    container(handler).run();

    return 0;

}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */