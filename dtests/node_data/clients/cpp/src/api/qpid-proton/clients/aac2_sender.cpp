/*
 * aac1_sender.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */

#include "messenger/SendingClient.h"

using dtests::proton::messenger::SendingClient;

int main(int argc, char** argv) {
	SendingClient client = SendingClient();

	return client.run(argc, argv);
}




