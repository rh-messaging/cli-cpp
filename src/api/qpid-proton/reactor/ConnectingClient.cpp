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
    
    bool conn_reconnect_custom = false;

    string conn_reconnect = "true";
    if (options.is_set("conn-reconnect")) {
        conn_reconnect = options["conn-reconnect"];
        std::transform(conn_reconnect.begin(), conn_reconnect.end(), conn_reconnect.begin(), ::tolower);
    }

    int32_t conn_reconnect_interval = -1;
    if (options.is_set("conn-reconnect-interval")) {
        conn_reconnect_interval = std::strtol(options["conn-reconnect-interval"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    int32_t conn_reconnect_limit = -1;
    if (options.is_set("conn-reconnect-limit")) {
        conn_reconnect_limit = std::strtol(options["conn-reconnect-limit"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    int32_t conn_reconnect_timeout = -1;
    if (options.is_set("conn-reconnect-timeout")) {
        conn_reconnect_timeout = std::strtol(options["conn-reconnect-timeout"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    uint32_t conn_reconnect_first = 0;
    if (options.is_set("conn-reconnect-first")) {
        conn_reconnect_first = std::strtoul(options["conn-reconnect-first"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    uint32_t conn_reconnect_increment = 100;
    if (options.is_set("conn-reconnect-increment")) {
        conn_reconnect_increment = std::strtoul(options["conn-reconnect-increment"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    bool conn_reconnect_doubling = true;
    if (options.is_set("conn-reconnect-doubling")) {
        if (options["conn-reconnect-doubling"] == "false") {
            conn_reconnect_doubling = false;
        }

        conn_reconnect_custom = true;
    }

    uint32_t max_frame_size = -1;
    if (options.is_set("conn-max-frame-size")) {
        max_frame_size = std::strtoul(options["conn-max-frame-size"].c_str(), NULL, 10);
    }
    
    OptionsSetter setter = OptionsSetter(options);
    
    int timeout = 0;
    if (options.is_set("timeout")) {
        timeout = static_cast<int> (options.get("timeout"));
    }

    ConnectorHandler handler = ConnectorHandler(
        address,
        user,
        password,
        sasl_mechanisms,
        timeout,
        conn_reconnect,
        conn_reconnect_interval,
        conn_reconnect_limit,
        conn_reconnect_timeout,
        conn_reconnect_first,
        conn_reconnect_increment,
        conn_reconnect_doubling,
        conn_reconnect_custom,
        max_frame_size
    );
    setMessageHandlerOptions(setter, handler);
    
    int connControl = setConnectionOptions(options);
    handler.setObjectControl(connControl);
    
    default_container(handler).run();

    return 0;
}


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

