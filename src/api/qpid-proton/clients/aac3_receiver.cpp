/*
 * aac1_sender.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */

#include <reactor/ReceivingClient.h>

using dtests::proton::reactor::ReceivingClient;

int main(int argc, char** argv)
{
    ReceivingClient client = ReceivingClient();

    return client.run(argc, argv);
}
