/*
 * aac3_sender.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */
#include <reactor/ReceivingClient.h>
#include <reactor/TxReceivingClient.h>

using dtests::proton::reactor::ReceivingClient;
using dtests::proton::reactor::TxReceivingClient;

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
        ReceivingClient client = ReceivingClient();
        return client.run(argc, argv);
    } else {
        TxReceivingClient client = TxReceivingClient();
        return client.run(argc, argv);
    }
}
