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

#include <cstdlib>

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
    
    string msg_action = "no-action";
    if(options.is_set("msg-action")) {
        msg_action = options["msg-action"];
    }

    int msg_action_size = 1;
    if(options.is_set("msg-action-size")) {
        msg_action_size = atoi(options["msg-action-size"].c_str());
    }

    string user = options["user"];
  
    string password = options["password"];
  
    string sasl_mechanisms = options["sasl-mechanisms"];

    string conn_reconnect = "default";
    if (options.is_set("conn-reconnect")) {
        conn_reconnect = options["conn-reconnect"];
    }

    OptionsSetter setter = OptionsSetter(options);
    
    int timeout = 0;
    if (options.is_set("timeout")) {
        timeout = static_cast<int> (options.get("timeout"));
    }

    bool process_reply_to = false;
    if (options.is_set("process-reply-to")) {
        process_reply_to = options.get("process-reply-to");
    }

    bool browse = false;
    if (options.is_set("recv-browse")) {
        browse = options.get("recv-browse");
    }

    ReceiverHandler handler = ReceiverHandler(address, msg_action, msg_action_size, user, password, sasl_mechanisms, timeout, conn_reconnect, process_reply_to, browse);
    default_container(handler).run();

    return 0;

}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
