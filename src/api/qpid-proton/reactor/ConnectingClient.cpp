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
		SimpleMessagingHandler &handler) const
{
	
	
}


int ConnectingClient::run(int argc, char** argv) const {
	const string usage = "usage: %prog [OPTION]... DIR [FILE]...";
	const string version = "1.0";
	const string desc = "C/C++ AMQ reactor client for Proton-C";

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
	
	SimpleMessagingHandler handler = SimpleMessagingHandler(address);
	container(handler).run();
	
	return 0;
}


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

