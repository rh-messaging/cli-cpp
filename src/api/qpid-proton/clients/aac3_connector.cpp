/*
 * aac1_sender.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */
#include <cstdlib>

#include <reactor/ConnectingClient.h>

using dtests::proton::reactor::ConnectingClient;

int main(int argc, char** argv)
{
    ConnectingClient client = ConnectingClient();

    return client.run(argc, argv);

}




