/*
 * aac3_sender.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */
#include <reactor/SendingClient.h>
#include <reactor/TxSendingClient.h>

using dtests::proton::reactor::SendingClient;
using dtests::proton::reactor::TxSendingClient;

int main(int argc, char** argv)
{
    int i = 0;
    bool tx_mode = false;
    std::string tx_opt = "--tx-";
    while (i < argc) {
        if (std::string(argv[i]).rfind("--tx-", 0) == 0) { // pos=0 limits the search to the prefix
	    tx_mode = true;
	    break;
        }
        i++;
    }

    if (tx_mode) {
        TxSendingClient client = TxSendingClient();
        return client.run(argc, argv);
    } else {
        SendingClient client = SendingClient();
        return client.run(argc, argv);
    }
}
