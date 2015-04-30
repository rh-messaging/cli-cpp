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

#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include <qpid/types/Variant.h>

#include <cstdlib>
#include <iostream>

#include <sstream>

using namespace qpid::messaging;
using namespace qpid::types;

using std::stringstream;
using std::string;

int main(int argc, char** argv) {
    const char* url = argc>1 ? argv[1] : "amqp:tcp:127.0.0.1:5672";
    const char* address = argc>2 ? argv[2] : "message_queue; {create: always}";
    std::string connectionOptions = argc > 3 ? argv[3] : "";

    Connection connection(url, connectionOptions);
    try {
        connection.open();
        Session session = connection.createSession();
        Sender sender = session.createSender(address);

        Message message;
        Variant::Map content;
        content["id"] = 987654321;
        content["name"] = "Widget";
        content["percent"] = 0.99;
        Variant::List colours;
        colours.push_back(Variant("red"));
        colours.push_back(Variant("green"));
        colours.push_back(Variant("white"));
        content["colours"] = colours;

        Variant::Map dimensions;
        dimensions["length"] = 10.2;
        dimensions["width"] = 5.1;
        dimensions["depth"] = 2.0;
        content["dimensions"]= dimensions;

        Variant::List part1;
        part1.push_back(Variant(1));
        part1.push_back(Variant(2));
        part1.push_back(Variant(5));

        Variant::List part2;
        part2.push_back(Variant(8));
        part2.push_back(Variant(2));
        part2.push_back(Variant(5));

        Variant::List parts;
        parts.push_back(part1);
        parts.push_back(part2);
        content["parts"]= parts;

        Variant::Map specs;
        specs["colours"] = colours;
        specs["dimensions"] = dimensions;
        specs["parts"] = parts;
        content["specs"] = specs;

        content["uuid"] = Uuid(true);

#ifdef NGETCONTENTOBJECT
        encode(content, message);
#else
        message.setContentObject(content);
#endif

        sender.send(message, true);

#ifdef NGETCONTENTOBJECT
        Variant::Map content_from_message;
        decode(message, content_from_message);
        std::cout << content_from_message << std::endl;
#else
        std::cout << message.getContentObject() << std::endl;
#endif

        connection.close();
        return 0;
    } catch(const std::exception& error) {
        std::cout << error.what() << std::endl;
        connection.close();
    }
    return 1;
}


