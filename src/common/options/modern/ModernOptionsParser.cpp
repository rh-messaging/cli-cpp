/*
 * ModernOptionsParser.cpp
 *
 *  Created on: Apr 20, 2015
 *      Author: opiske
 */

#include <stdint.h>
#include <cstdlib>

#include "options/modern/ModernOptionsParser.h"

#define MIN_MAX_FRAME_SIZE 512

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

    add_option("--conn-reconnect")
            .dest("conn-reconnect")
            .help("client RECONNECT settings (default: default)")
            .metavar("RECONNECT");

    // max, max_delay
    add_option("--conn-reconnect-interval")
            .dest("conn-reconnect-interval")
            .help("client reconnect INTERVAL (only supported with \"custom\" reconnect, default: -1)")
            .metavar("INTERVAL");

    // max_retries
    add_option("--conn-reconnect-limit")
            .dest("conn-reconnect-limit")
            .help("client reconnect LIMIT (only supported with \"custom\" reconnect, default: -1)")
            .metavar("LIMIT");

    // timeout
    add_option("--conn-reconnect-timeout")
            .dest("conn-reconnect-timeout")
            .help("client reconnect TIMEOUT (only supported with \"custom\" reconnect, default: -1)")
            .metavar("TIMEOUT");

/*********************** Reactive C++ API client extras ***********************/
   add_option("--conn-reconnect-first")
            .dest("conn-reconnect-first")
            .help("client reconnect FIRST (only supported with \"custom\" reconnect, default: 0)")
            .metavar("FIRST");

   add_option("--conn-reconnect-increment")
            .dest("conn-reconnect-increment")
            .help("client reconnect INCREMENT (only supported with \"custom\" reconnect, default: 100)")
            .metavar("INCREMENT");

    add_option("--conn-reconnect-doubling")
            .dest("conn-reconnect-doubling")
            .help("client reconnect DOUBLING (only supported with \"custom\" reconnect, default: true)")
            .metavar("DOUBLING");
/******************************************************************************/
    

    std::stringstream sstm;
    sstm << "maximum frame SIZE (" << MIN_MAX_FRAME_SIZE << " - " << UINT32_MAX << ", default: " << UINT32_MAX << ")";
    add_option("--conn-max-frame-size")
            .dest("conn-max-frame-size")
            .help(sstm.str())
            .type("int")
            .metavar("SIZE");

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

    unsigned long max_frame_size_opt = strtoul(options["conn-max-frame-size"].c_str(), NULL, 10);
    if (options.is_set("conn-max-frame-size") && (max_frame_size_opt > UINT32_MAX || max_frame_size_opt < MIN_MAX_FRAME_SIZE)) {
        print_help();
        std::stringstream sstm;
        sstm << "Maximum frame size " << options["conn-max-frame-size"] << " is out of range (" << MIN_MAX_FRAME_SIZE << " - " << UINT32_MAX << ")";
        error(sstm.str());
    }
}
