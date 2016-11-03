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

using proton::default_container;

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

int ConnectingClient::setConnectionOptions(const optparse::Values &options) const
{
    string value = options["obj-ctrl"];
    int connControl = 0;
    
    size_t pos = value.find("C");
    if (pos != string::npos) {
        connControl |= CONNECTION; 
    }
    
    pos = value.find("E");
    if (pos != string::npos) {
        connControl |= SESSION;
    }
    
    pos = value.find("S");
    if (pos != string::npos) {
        connControl |= SENDER;
    }
    
    pos = value.find("R");
    if (pos != string::npos) {
        connControl |= RECEIVER;
    }
    
    return connControl;
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

    string user = options["user"];

    string password = options["password"];
    
    string sasl_mechanisms = options["sasl-mechanisms"];

    OptionsSetter setter = OptionsSetter(options);
    
    int timeout = 0;
    if (options.is_set("timeout")) {
        timeout = static_cast<int> (options.get("timeout"));
    }

    ConnectorHandler handler = ConnectorHandler(address, user, password, sasl_mechanisms, timeout);
    setMessageHandlerOptions(setter, handler);
    
    int connControl = setConnectionOptions(options);
    handler.setObjectControl(connControl);
    
    default_container(handler).run();

    return 0;
}


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

