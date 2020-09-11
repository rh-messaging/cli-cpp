/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */

#include <qpid/messaging/Address.h>
#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>

#include <cstdlib>
#include <iostream>

#include <sstream>

using namespace qpid::messaging;

using std::stringstream;
using std::string;

int main(int argc, char** argv) {
    const char* url = argc>1 ? argv[1] : "amqp:tcp:127.0.0.1:5672";
    std::string connectionOptions = argc > 2 ? argv[2] : "";
    std::string address = argc > 3 ? argv[3] : "service_queue";
    
    Connection connection(url, connectionOptions);
     try {
        connection.open();
        Session session = connection.createSession();

        Sender sender = session.createSender(address);

        //create temp queue & receiver...
#ifdef NGETADDRESS
        Address responseQueue("#response-queue; {create:always, delete:always}");
        Receiver receiver = session.createReceiver(responseQueue);
#else
        Receiver receiver = session.createReceiver("#");
        Address responseQueue = receiver.getAddress();
#endif

	// Now send some messages ...
	string s[] = {
            "Twas brillig, and the slithy toves",
            "Did gire and gymble in the wabe.",
            "All mimsy were the borogroves,",
            "And the mome raths outgrabe."
        };

    	Message request;
        request.setReplyTo(responseQueue);
	for (int i=0; i<4; i++) {
            request.setContent(s[i]);
            sender.send(request);
            Message response = receiver.fetch();
            std::cout << request.getContent() << " -> " << response.getContent() << std::endl;
	}
        connection.close();
        return 0;
    } catch(const std::exception& error) {
        std::cout << error.what() << std::endl;
        connection.close();
    }
    return 1;
}


