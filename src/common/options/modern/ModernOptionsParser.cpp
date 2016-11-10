/*
 * ModernOptionsParser.cpp
 *
 *  Created on: Apr 20, 2015
 *      Author: opiske
 */

#include "options/modern/ModernOptionsParser.h"

using namespace dtests::common;

ModernOptionsParser::ModernOptionsParser()
    : super()
{
    add_option("-b", "--broker-url")
            .dest("broker-url")
            .help("URL of broker to connect to")
            .metavar("URL");

    add_option("-u", "--user")
            .dest("user")
            .help("authenticate as USER")
            .metavar("USER");

    add_option("-p", "--password")
            .dest("password")
            .help("authenticate with PASSWORD")
            .metavar("PASSWORD");

    add_option("--sasl-mechanisms")
            .dest("sasl-mechanisms")
            .help("specify the allowed MECHANISMS for use on the connection")
            .metavar("MECHANISMS");

    add_option("--client-log-level")
            .dest("client-log-level")
            .help("log LEVEL for the client")
            .metavar("LEVEL");

    add_option("--close-sleep")
            .dest("close-sleep")
            .help("sleep before closing the connection for the specified TIME (seconds)")
            .type("int")
            .metavar("TIME");

#ifdef ENABLE_BLOCKING
    set_defaults("blocking", false);
    add_option("--blocking")
            .action("store_true")
            .dest("blocking")
            .type("bool")
            .help("whether to use blocking or not");
#endif // ENABLE_BLOCKING
}

ModernOptionsParser::~ModernOptionsParser()
{
    // TODO Auto-generated destructor stub
}

void ModernOptionsParser::validate(const Values &options) const
{
    if (!options.is_set("broker-url")) {
        print_help();
        error("Broker URL is not set");
    }
}
