/*
 * aac1_sender.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */
#include <cstdlib>

#include "reactor/SendingClient.h"

using dtests::proton::reactor::SendingClient;

int main(int argc, char** argv) {
#ifdef QPID_PROTON_WITH_MESSAGING_API
	SendingClient client = SendingClient();

	return client.run(argc, argv);
#else
	return EXIT_SUCCESS;
#endif
}




