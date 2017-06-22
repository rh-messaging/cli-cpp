/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ConnectorOptionsParser.cpp
 * Author: opiske
 * 
 * Created on October 14, 2015, 1:35 PM
 */

#include "ConnectorOptionsParser.h"

using namespace dtests::common;

ConnectorOptionsParser::ConnectorOptionsParser()
    : super()
{
    /*
     * TODO: implement option groups
     */
    add_option("-c", "--count")
            .dest("count")
            .type("int")
            .help("stop after count messages")
            .metavar("COUNT");
    
    add_option("--obj-ctrl")
            .dest("obj-ctrl")
            .help("optional creation object control based on <object-ids> syntax C/E/S/R stands for Connection, sEssion, Sender, Receiver")
            .metavar("OBJCTRL");
    
    add_option("--timeout")
            .dest("timeout")
            .help("exit after the specified time")
            .type("int");

#ifdef UNSUPPORTED_CONNECTOR_OPTS
    // Control options


    add_option("--sync-mode")
            .dest("msg-correlation-id")
            .help("optional action synchronization mode: none/session/action")
            .metavar("ID");


    // Logging options
    add_option("--log-lib")
            .dest("log-lib")
            .help("enable client library logging (default none)")
            .metavar("LOG-LIB");

    add_option("--log-stats")
            .dest("log-stats")
            .help("enable client library logging (default none)")
            .metavar("LOG-STATS");

    // Connection options
    add_option("--conn-urls")
            .dest("log-stats")
            .help("define connection urls")
            .metavar("CONN_URLS");

    add_option("--conn-reconnect")
            .dest("conn-reconnect")
            .help("client reconnect settings (default default)")
            .metavar("CONN_RECONNECT");

    add_option("--conn-reconnect-interval")
            .dest("conn-reconnect-interval")
            .type("int")
            .help("client reconnect interval (only supported with 'custom'reconnect, default none)")
            .metavar("CONN_RECONNECT_INTERVAL");

    add_option("--conn-reconnect-limit")
            .dest("conn-reconnect-limit")
            .type("int")
            .help("client reconnect limit (only supported with 'custom'reconnect, default 99)")
            .metavar("CONN_RECONNECT_LIMIT");

    add_option("--conn-reconnect-timeout")
            .dest("conn-reconnect-timeout")
            .type("int")
            .help("client reconnect timeout (only supported with 'custom'reconnect, default none)")
            .metavar("CONN_RECONNECT_TIMEOUT");

    add_option("--conn-heartbeat")
            .dest("conn-heartbeat")
            .type("int")
            .help("enable and set connection heartbeat (seconds)")
            .metavar("CONN_HEARTBEAT");

    add_option("--conn-ssl-certificate")
            .dest("conn-heartbeat")
            .help("path to client certificate (PEM format), enables client authentication")
            .metavar("CONN_SSL_CERTIFICATE");

    add_option("--conn-ssl-private-key")
            .dest("conn-ssl-private-key")
            .help("path to client private key (PEM format), conn-ssl-certificate must be given")
            .metavar("CONN_SSL_PRIVATE_KEY");

    add_option("--conn-ssl-password")
            .dest("conn-ssl-password")
            .help("client's certificate database password")
            .metavar("CONN_SSL_PASSWORD");

    add_option("--conn-ssl-trust-store")
            .dest("conn-ssl-trust-store")
            .help("path to client trust store (PEM format), conn-ssl-certificate must be given")
            .metavar("CONN_SSL_TRUST_STORE");

    add_option("--conn-ssl-verify-peer")
            .dest("cconn-ssl-verify-peer")
            .help("verifies server vertificate, conn-ssl-certificate and trusted db path needs to be specified (PEM format)");

    add_option("--conn-ssl-verify-peer-name")
            .dest("conn-ssl-verify-peer-name")
            .help("verifies server vertificate, conn-ssl-certificate and trusted db path needs to be specified (PEM format)");

    add_option("--conn-handler")
            .dest("conn-handler")
            .help("define custom connection handler")
            .metavar("CONN_HANDLER");

    // Connector options
    
#endif // UNSUPPORTED_CONNECTOR_OPTS

}

ConnectorOptionsParser::~ConnectorOptionsParser()
{
}

void ConnectorOptionsParser::validate(const Values& options) const
{
    super::validate(options);
}
