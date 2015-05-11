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
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Session.h>
#include <qpid/messaging/Address.h>

#include <cstdlib>
#include <iostream>
#include <typeinfo>

#include "OptionParser.h"
#include "Formatter.h"
#include "Utils.h"

#ifdef ENABLE_MODERN
#include "formatter/AbstractFormatter.h"
#include "formatter/AbstractDecoder.h"
#include "formatter/Writer.h"
#include "formatter/DictWriter.h"
#include "formatter/QpidDecoder.h"

using dtests::qpid::messaging::QpidDecoder;

#endif // ENABLE_MODERN

using namespace qpid::messaging;
using namespace qpid::types;

struct Options : OptionParser
{
    std::string url;
    std::string address;
    std::string connectionOptions;
    std::string accept;
    int timeout;
    bool forever;
    int count;
    int duration;
    int capacity;
    int close_sleep;
    std::string logMsgs;
    std::string logStats;
    int tx_batch_size;
    std::string tx_action;
    std::string sync_mode;
    bool verbose;

    Options()
        : OptionParser("Usage: drain [OPTIONS] ADDRESS", "Drains messages from the specified address"),
          url("127.0.0.1"),
          timeout(0),
          forever(false),
          count(0),
          duration(0),
          capacity(-1),
          close_sleep(0),
          logMsgs("upstream"),
          logStats(""),
          tx_batch_size(0),
          tx_action("commit"),
          sync_mode("none"),
          verbose(false)
    {
        add("broker,b", url, "url of broker to connect to");
        add("timeout,t", timeout, "timeout in seconds to wait before exiting");
        add("forever,f", forever, "ignore timeout and wait forever");
        add("count,c", count, "number of messages to read before exiting");
        add("duration", duration, "message actions total duration (defines msg-rate together with count)");
        add("accept,a", accept, "action on acquired message");
        add("capacity", capacity, "set receiver's capacity");
        add("close-sleep,s", close_sleep, "sleep before sender/receiver/session/connection.close()");
        add("connection-options", connectionOptions, "connection options string in the form {name1:value1, name2:value2}");
        add("log-msgs", logMsgs, "message[s] reporting style (dict|body|upstream|none)");
        add("log-stats", logStats, "report various statistic/debug information");
        add("tx-batch-size", tx_batch_size, "transactional mode: batch message count size (negative skips tx-action before exit)");
        add("tx-action", tx_action, "transactional action at the end of tx batch");
        add("sync-mode", sync_mode, "synchronization mode: none/session/action");
        add("verbose", verbose, "verbose AMQP message output");
    }

    Duration getTimeout()
    {
        if (forever) return Duration::FOREVER;
        else return timeout*Duration::SECOND;
    }

    int getCount()
    {
        return count;
    }

    bool checkAddress()
    {
        if (getArguments().empty()) {
            error("Address is required");
            return false;
        } else {
            address = getArguments()[0];
            return true;
        }
    }
};


int main(int argc, char** argv)
{
    // set stdout/err non-buffered operations
    if (get_env("PYTHONUNBUFFERED").length() > 0)
        set_stdx_unbuffered();

    // timestamping static data
    double tsdata [ 10 ] = { -1, -1, -1, -1, -1, -1, -1, 9.9e+99, 0, 0 };
    double *ptsdata = NULL;

    Options options;
#ifdef ENABLE_MODERN
#warning Using unstable code
    AbstractFormatter formatter = AbstractFormatter();
    
    std::ostringstream stream;
    DictWriter writer = DictWriter(&stream);
#else
    Formatter formatter;
#endif
    if (options.parse(argc, argv) && options.checkAddress()) {

        // init timestamping
        ptsdata = ts_init((double *) &tsdata, options.logStats);

        ts_snap_store(ptsdata, 'B', options.logStats);
        Connection connection(options.url, options.connectionOptions);
        try {
            connection.open();
            ts_snap_store(ptsdata, 'C', options.logStats);
            Session session;
            if (options.tx_batch_size != 0)
                session = connection.createTransactionalSession();
            else
                session = connection.createSession();
            ts_snap_store(ptsdata, 'D', options.logStats);
            Receiver receiver = session.createReceiver(options.address);
            // set receiver's capacity (if defined as > -1)
            if (options.capacity > -1)
                receiver.setCapacity(options.capacity);

            Duration timeout = options.getTimeout();
            int count = options.getCount();
            Message message;
            long int message_size = 0;
            int i = 0;
            bool tx_batch_flag = (options.tx_batch_size != 0);
            double ts = get_time();

            ts_snap_store(ptsdata, 'E', options.logStats);

            while (receiver.fetch(message, timeout)) {

                if( (ptsdata != NULL) && 
                    (message.getProperties()["ts"].getType() != VAR_VOID) ) {
                    ts_snap_store(ptsdata, 'L', options.logStats,
                                  (double)message.getProperties()["ts"]);
                }

                if (options.sync_mode == "action")
                    // call sync after every action (fetch())
                    session.sync();

                if (options.logMsgs == "body") {
                    std::cout << message.getContent() << std::endl;
                } else if (options.logMsgs == "dict") {
#ifdef ENABLE_MODERN
                    QpidDecoder decoder = QpidDecoder(message); 
                                        
                    formatter.printMessage(&decoder, &writer);
                    
                    writer.endLine();
                    std::cout << writer.toString();
                    
                    
#else
                    formatter.printMessageAsDict(message);
#endif
      
                } else if (options.logMsgs == "upstream") {
#ifdef ENABLE_MODERN
                    QpidDecoder decoder = QpidDecoder(message); 
                                        
                    formatter.printMessage(&decoder, &writer);
                    
                    writer.endLine();
                    std::cout << writer.toString();
                    
#else
                    formatter.printMessage(message, options.verbose);
#endif
                }

                // define message rate --count + --duration
                if (options.duration > 0)
                    sleep4next(ts, options.count, options.duration, i+1);

                if (options.accept == "reject")
                    session.reject(message);
                else if (options.accept == "release")
                    session.release(message);
                else if (options.accept != "noack")
                    session.acknowledge(message);

                if(options.tx_batch_size != 0) {
                    // transactions enabled
                    if( (((i+1) % abs(options.tx_batch_size)) == 0) ) {
                        // end of transactional batch
                        if (options.tx_action == "commit") {
                            session.commit();
                            tx_batch_flag = false;
                        }
                        else if (options.tx_action == "rollback") {
                            session.rollback();
                            tx_batch_flag = false;
                        }
                    } else {
                        tx_batch_flag = true;
                    }
                }

                // define message rate --count + --duration
                if (options.duration < 0)
                    sleep4next(ts, options.count, abs(options.duration), i+1);

                // get message size
                if (i == 0)
                    message_size = message.getContent().size();

                if ((++i == count) && count)
                    break;
            }
            ts_snap_store(ptsdata, 'F', options.logStats);

            // end of message stream, tx-action if missing and for positive batch size only
            if( (options.tx_batch_size > 0) && (tx_batch_flag == true) ) {
                if (options.tx_action == "commit")
                    session.commit();
                else if (options.tx_action == "rollback")
                    session.rollback();
            }

            // sender/receiver/session/connection.close() sleep
            if (options.close_sleep)
                msleep(options.close_sleep * 1000);

            // closing receiver/session/connection
            receiver.close();
            if (options.sync_mode == "session")
                // call sync before session.close()
                session.sync();
            session.close();
            connection.close();
            ts_snap_store(ptsdata, 'G', options.logStats);
            // report timestamping before success exit
            if (ptsdata != NULL)
                std::cout << "STATS " << ts_report(ptsdata, i, message_size, 0)
                          << std::endl;
            return 0;
        } catch(const std::exception& error) {
            std::cerr << error.what() << std::endl;
            connection.close();
            ts_snap_store(ptsdata, 'G', options.logStats);
            // report timestamping before failure exit
            if (ptsdata != NULL)
                std::cout << "STATS " << ts_report(ptsdata, -1, -1, 1)
                          << std::endl;
        }
    }
    return 1;
}
