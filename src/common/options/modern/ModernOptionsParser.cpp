/*
 * ModernOptionsParser.cpp
 *
 *  Created on: Apr 20, 2015
 *      Author: opiske
 */

#include "options/modern/ModernOptionsParser.h"

ModernOptionsParser::ModernOptionsParser()
	: super()
{
	add_option("-b", "--broker-url")
				.dest("broker-url")
		  	  	.help("url of broker to connect to");

	add_option("--client-log-level")
				.dest("client-log-level")
				.help("log level for the client");

	add_option("--close-sleep")
				.dest("close-sleep")
				.help("sleep before closing the connection")
				.type("int");

#ifdef ENABLE_BLOCKING
	set_defaults("blocking", false);
	add_option("--blocking")
					.action("store_true")
					.dest("blocking")
					.type("bool")
					.help("whether to use blocking or not");
#endif // ENABLE_BLOCKING
}

ModernOptionsParser::~ModernOptionsParser() {
	// TODO Auto-generated destructor stub
}

void ModernOptionsParser::validate(const Values &options) const {
	if (!options.is_set("broker-url")) {
		print_help();
		error("Broker URL is not set");
	}
}
