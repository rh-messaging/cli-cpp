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
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include <qpid/types/Variant.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>

#include <boost/format.hpp>

#include "OptionParser.h"
#include "Utils.h"
#include "formatter/FormatUtil.h"

using namespace qpid::messaging;
using namespace qpid::types;

using dtests::qpid::messaging::printMessageDict;
using dtests::qpid::messaging::printMessageUpstream;
using dtests::qpid::messaging::printStatistics;

typedef std::vector<std::string> string_vector;

struct Options : OptionParser
{
    std::string url;
    std::string address;
    int timeout;
    int count;
    int duration;
    std::string id;
    std::string replyto;
    std::string subject;
    string_vector properties;
    string_vector entries;
    std::string content;
    std::string content_from_file;
    std::string connectionOptions;
    std::string logMsgs;
    std::string logStats;
    int tx_batch_size;
    std::string tx_action;
    std::string sync_mode;
    std::string durable;
    int ttl;
    int priority;
    int capacity;
    int close_sleep;
    std::string correlationId;
    std::string userId;


    Options()
        : OptionParser("Usage: spout [OPTIONS] ADDRESS", "Send messages to the specified address"),
          url("127.0.0.1"),
          timeout(0),
          count(1),
          duration(0),
          logMsgs(""),
          logStats(""),
          tx_batch_size(0),
          tx_action("commit"),
          sync_mode("session"),
          durable(""),
          ttl(-1),
          priority(-1),
          capacity(-1),
          close_sleep(0),
          correlationId(""),
          userId("")
    {
        add("broker,b", url, "url of broker to connect to");
        add("timeout,t", timeout, "exit after the specified time");
        add("count,c", count, "stop after count messages have been sent, zero disables");
        add("duration", duration, "message actions total duration (defines msg-rate together with count)");
        add("id,i", id, "use the supplied id instead of generating one");
        add("reply-to", replyto, "specify reply-to address");
        add("subject,S", subject, "specify message subject");
        add("property,P", properties, "specify message property");
        add("map,M", entries, "specify entry for map content");
        add("content", content, "specify textual content");
        add("content-from-file", content_from_file, "specify file name to load content from");
        add("connection-options", connectionOptions, "connection options string in the form {name1:value1, name2:value2}");
        add("log-msgs", logMsgs, "message[s] reporting style (dict|body|upstream|none)");
        add("log-stats", logStats, "report various statistic/debug information");
        add("tx-batch-size", tx_batch_size, "transactional mode: batch message count size (negative skips tx-action before exit)");
        add("tx-action", tx_action, "transactional action at the end of tx batch");
        add("sync-mode", sync_mode, "synchronization mode: none/session/action");
        add("durable", durable, "send durable messages yes/no");
        add("ttl", ttl, "message time-to-live (ms)");
        add("priority", priority, "message priority");
        add("capacity", capacity, "set sender's capacity");
        add("close-sleep,s", close_sleep, "sleep before sender/receiver/session/connection.close()");
        add("correlation-id", correlationId, "message correlation id");
        add("user-id", userId, "message user id");
    }

    static bool nameval(const std::string& in, std::string& name, std::string& value, std::string& separator)
    {
        std::string::size_type i = in.find("=");
        separator = "=";
        if (i == std::string::npos) {
            std::string::size_type i = in.find("~");
            separator = "~";
            if (i == std::string::npos) {
              name = in;
              return false;
            } else {
              name = in.substr(0, i);
              if (i+1 < in.size()) {
                  value = in.substr(i+1);
                  return true;
              } else {
                  return false;
              }
            }
        } else {
            name = in.substr(0, i);
            if (i+1 < in.size()) {
                value = in.substr(i+1);
                return true;
            } else {
                return false;
            }
        }
    }

    static void setProperty(Message& message, const std::string& property)
    {
        std::string name;
        std::string value;
        std::string separator;
        if (nameval(property, name, value, separator)) {
            if (separator == "~") {
              message.getProperties()[name] = Variant().parse(value);
              message.getProperties()[name].setEncoding("utf8");
            } else {
              message.getProperties()[name] = Variant(value);
              message.getProperties()[name].setEncoding("utf8");
            }
        } else {
            message.getProperties()[name] = Variant();
        }
    }

    void setProperties(Message& message) const
    {
        for (string_vector::const_iterator i = properties.begin(); i != properties.end(); ++i) {
            setProperty(message, *i);
        }
    }

    void setEntries(Variant::Map& content) const
    {
        for (string_vector::const_iterator i = entries.begin(); i != entries.end(); ++i) {
            std::string name;
            std::string value;
            std::string separator;
            if (nameval(*i, name, value, separator)) {
                if (separator == "~") {
                  content[name] = Variant().parse(value);
                } else {
                  content[name] = value;
                }
            } else {
                content[name] = Variant();
            }
        }
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

Variant::Map getSenderStats(Sender &snd) {
   Variant::Map stats;
   Variant::Map sender;
   sender["capacity"] = snd.getCapacity();
   sender["available"] = snd.getAvailable();
   sender["unsettled"] = snd.getUnsettled();
   stats["sender"] = sender;
   stats["timestamp"] = get_time();
   return stats;
}

int main(int argc, char** argv)
{
    // set stdout/err non-buffered operations
    if (get_env("PYTHONUNBUFFERED").length() > 0)
        set_stdx_unbuffered();

    // timestamping static data
    double tsdata [ 10 ] = { -1, -1, -1, -1, -1, -1, -1, 9.9e+99, 0, 0 };
    double *ptsdata = NULL;

    Options options;
    
    if (options.parse(argc, argv) && options.checkAddress()) {

        // init timestamping
        ptsdata = ts_init((double *) &tsdata, options.logStats);

        bool msg_content_fmt = ( (options.content.find('%') != std::string::npos) && \
                                 (options.content.find('d') != std::string::npos) && \
                                 (options.content.find('d', options.content.find('%')) != std::string::npos) );
        // convert options.durable to lowcase
        std::transform(options.durable.begin(), options.durable.end(),
                       options.durable.begin(), ::tolower);
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
            Sender sender = session.createSender(options.address);
            // set sender's capacity (if defined as > -1)
            if (options.capacity > -1)
                sender.setCapacity(options.capacity);

            Message message;
            options.setProperties(message);
            if (options.entries.size()) {
                Variant::Map content;
                options.setEntries(content);
                message.setContentType("amqp/map");
#ifdef NGETCONTENTOBJECT
                encode(content, message);
#else
                message.getContentObject() = content;
#endif
            } else if (options.content.size()) {
#ifdef NGETCONTENTOBJECT
                message.setContent(options.content);
#else
                message.getContentObject() = options.content;
                message.getContentObject().setEncoding("utf8");
#endif
                message.setContentType("text/plain");
            } else if (options.content_from_file.size()) {
              // load content
              std::string text_buffer;
              std::ifstream content_file;
              try {
                content_file.open(options.content_from_file.c_str());
                if (content_file.is_open()) {
                  // assing a content of file from initial iterator (begin)
                  // to EOF iterator
                  text_buffer.assign(
                    std::istreambuf_iterator<char>(content_file),
                    std::istreambuf_iterator<char>()
                  );
                }
                message.setContent(text_buffer);
              } catch(const std::ios_base::failure & exc) {
                // TODO it does not throw Exception, resolve
                std::cerr << exc.what();
                message.setContent("");
              }

              // set content
              //
                message.setContentType("text/plain");
            }
            // set message subject
            if (!options.subject.empty()) {
               message.setSubject(options.subject);
            }
            /* Durability settings */
            // default = "" supports for 3 state durability
            if ( (options.durable == "yes") || (options.durable == "true") ||
             (options.durable == "True") ) {
              message.setDurable(true);
            }
            else if ( (options.durable == "no") ||
             (options.durable == "false") || (options.durable == "True") ) {
                message.setDurable(false);
            }

            if (options.ttl > -1)
                message.setTtl(Duration(options.ttl));
            if (options.priority > -1)
                message.setPriority(options.priority);
            bool tx_batch_flag = (options.tx_batch_size != 0);
            std::time_t start = std::time(0);
            double ts = get_time();
            std::string sid = options.id.empty() ? Uuid(true).str() : options.id;
            ts_snap_store(ptsdata, 'E', options.logStats);
            for (int count = 0;
                (count < options.count || options.count == 0) &&
                (options.timeout == 0 || std::difftime(std::time(0), start) < options.timeout);
                count++) {

                // set message headers and properties
                if (!options.replyto.empty()) {
                   message.setReplyTo(Address(options.replyto));
                }
                std::stringstream spoutid;
                spoutid << sid << ":" << count;
                message.getProperties()["spout_id"] = spoutid.str();

                if (options.id != "")
                  message.setMessageId(options.id);
                if (options.correlationId != "")
                  message.setCorrelationId(options.correlationId);
                if (options.userId != "")
                  message.setUserId(options.userId);

                // assign formatted message content
                if (msg_content_fmt) {
#ifdef NGETCONTENTOBJECT
                 message.setContent( boost::str(boost::format(options.content) % count) );
#else
                 message.getContentObject() = boost::str(boost::format(options.content) % count);
#endif
                }
                if (ptsdata != NULL) {
                   message.getProperties()["ts"] = get_time();
                }
                sender.send(message);

                if (options.logStats.find("endpoints") != std::string::npos) {
                  Variant::Map stats = getSenderStats(sender);
		  
		  printStatistics(message, stats);
                }

                if (options.sync_mode == "action")
                    // call sync after every action (send())
                    session.sync();

                if (options.logMsgs == "body") {
                    std::cout << message.getContent() << std::endl;
                } else if (options.logMsgs == "dict") {
		    printMessageDict(message);
                } else if (options.logMsgs == "upstream") {
		    printMessageUpstream(message, false); 
                }

                // define message rate --count + --duration
                if (options.duration > 0)
                    sleep4next(ts, options.count, options.duration, count+1);

                if (options.tx_batch_size != 0) {
                    // transactions enabled
                    if (((count+1) % abs(options.tx_batch_size)) == 0) {
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
                    sleep4next(ts, options.count, abs(options.duration), count+1);
            }
            ts_snap_store(ptsdata, 'F', options.logStats);
            // end of message stream, tx-action if missing and for positive batch size only
            if( (options.tx_batch_size > 0) && (tx_batch_flag == true) ) {
                if (options.tx_action == "commit")
                    session.commit();
                else if (options.tx_action == "rollback")
                    session.rollback();
            }
            if (options.sync_mode == "session")
                // call sync before session.close()
                session.sync();

            // sender/receiver/session/connection.close() sleep
            if (options.close_sleep)
                msleep(options.close_sleep * 1000);

            if (options.logStats.find("endpoints") != std::string::npos) {
                Variant::Map stats = getSenderStats(sender);
                printStatistics(message, stats);
            }

            session.close();
            connection.close();
            ts_snap_store(ptsdata, 'G', options.logStats);
            // report timestamping before success exit
            if (ptsdata != NULL)
                std::cout << "STATS " << ts_report(ptsdata, options.count,
                                                   message.getContent().size(), 0)
                      << std::endl;
            return 0;
        } catch(const std::exception& error) {
            std::cerr << error.what() << std::endl;
            connection.close();
            ts_snap_store(ptsdata, 'G', options.logStats);
        }
    }
    // report timestamping before failure exit
    if (ptsdata != NULL)
        std::cout << "STATS " << ts_report(ptsdata, options.count,
                                           options.content.size(), 1)
                  << std::endl;
    return 1;
}


