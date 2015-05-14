/*
 * aac1_sender.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */

#include "reactor/SendingClient.h"

using dtests::proton::reactor::SendingClient;

int main(int argc, char** argv) {
	SendingClient client = SendingClient();

	return client.run(argc, argv);
}




