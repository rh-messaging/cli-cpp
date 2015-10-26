/*
 * aac1_sender.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */
#include <cstdlib>

#include "messenger/SendingClient.h"

using dtests::proton::messenger::SendingClient;

int main(int argc, char** argv)
{
#ifdef QPID_PROTON_WITH_MESSAGING_API
    SendingClient client = SendingClient();

    return client.run(argc, argv);
#else
    return EXIT_SUCCESS;
#endif
}




