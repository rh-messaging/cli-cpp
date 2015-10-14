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

ConnectorOptionsParser::ConnectorOptionsParser()
	: super()
{
	
	add_option("-c", "--count")
			.dest("count")
			.type("int")
			.help("stop after count messages")
			.metavar("COUNT");
	
	add_option("--connection-options")
			.dest("connection-options")
			.help("optional connection options (authentication, reconnect, etc.)")
			.metavar("CONNOPTS");

	add_option("--address")
			.dest("address")
			.help("optional AMQP address used for address creation / deletion / check of existance")
			.metavar("ADDRESS");
	
	add_option("--obj-ctrl")
			.dest("obj-ctrl")
			.help("optional creation object control based on <object-ids> syntax C/E/S/R stands for Connection, sEssion, Sender, Receiver")
			.metavar("OBJCTRL");

	add_option("--sync-mode")
			.dest("msg-correlation-id")
			.help("optional action synchronization mode: none/session/action")
			.metavar("ID");

}

ConnectorOptionsParser::~ConnectorOptionsParser()
{
}



void ConnectorOptionsParser::validate(const Values& options) const {
	super::validate(options);
}