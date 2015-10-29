/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConnectingClient.cpp
 * Author: opiske
 * 
 * Created on October 14, 2015, 1:24 PM
 */

#include "ConnectingClient.h"

using proton::container;

namespace dtests {
namespace proton {
namespace reactor {

ConnectingClient::ConnectingClient()
    : super()
{
}

ConnectingClient::~ConnectingClient()
{
}

void ConnectingClient::setMessageHandlerOptions(const OptionsSetter &setter,
        ConnectorHandler &handler) const
{
    setter.setNumber("count", &handler, &ConnectorHandler::setCount, 1);
}

int ConnectingClient::run(int argc, char** argv) const
{
    const string usage = "usage: %prog [OPTION]... DIR [FILE]...";
    const string version = "1.0";
    const string desc = "C/C++ AMQ reactive API connecting client for Qpid Proton";

    ConnectorOptionsParser parser = ConnectorOptionsParser();

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

    ConnectorHandler handler = ConnectorHandler(address);
    setMessageHandlerOptions(setter, handler);

    container(handler).run();

    return 0;
}


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

