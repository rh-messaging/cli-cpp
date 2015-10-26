/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReceiverHandler.cpp
 * Author: opiske
 * 
 * Created on October 19, 2015, 4:43 PM
 */

#include "ReceiverHandler.h"
#include <sstream>

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;
using namespace dtests::common::log;

ReceiverHandler::ReceiverHandler(const string &url)
    : super(url)
{
}

ReceiverHandler::~ReceiverHandler()
{
}

void ReceiverHandler::on_start(event &e)
{
    logger(debug) << "Starting messaging handler";

    logger(debug) << "Creating a receiver";
    e.container().open_receiver(broker_url);
}

void ReceiverHandler::on_message(event& e)
{
    ReactorDecoder decoder = ReactorDecoder(e.message());

    std::ostringstream stream;
    DictWriter writer = DictWriter(&stream);

    DictFormatter formatter = DictFormatter();
    formatter.printMessage(&decoder, &writer);

    writer.endLine();
    std::cout << writer.toString();
}

void ReceiverHandler::on_accepted(event& e)
{
    logger(debug) << "Accepted: " << e.name();
}

void ReceiverHandler::on_disconnected(event& e)
{
    logger(debug) << "Accepted: " << e.name();
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */