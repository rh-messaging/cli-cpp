// aac0_sender client

// TODO: --log-lib wiring
// TODO: --duration <float>
// TODO: --content-type <typeid>

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
#include "formatter/Formatter.h"
#include "Utils.h"
#include "formatter/FormatUtil.h"

#define MIN_MAX_FRAME_SIZE 4096

using namespace qpid::messaging;
using namespace qpid::types;

using dtests::qpid::messaging::printMessageDict;
using dtests::qpid::messaging::printStatistics;

typedef std::vector<std::string> string_vector;

struct Options : OptionParser
{
    std::string broker;
    std::string conn_opts;
    std::string max_frame_size;
    std::string address;

    int timeout;
    int count;
    int duration;
    std::string duration_mode;
    
    int close_sleep;
    
    std::string msg_id;
    std::string msg_replyto;
    std::string msg_subject;
    string_vector msg_properties;
    std::string msg_durable;
    int msg_ttl;
    int msg_priority;
    std::string msg_correlation_id;
    std::string msg_user_id;
    std::string msg_content_type;
    std::string msg_content;
    std::string msg_content_from_file;
    
    string_vector list_entries;
    string_vector map_entries;
    
    std::string content_type;

    int tx_size;
    std::string tx_action;
    std::string tx_el_action;

    std::string sync_mode;
    int capacity;
    
    std::string log_msgs;
    std::string log_stats;
    std::string log_lib;


    Options()
        : OptionParser("Usage: aac0_sender [OPTIONS]", "Send messages to the specified address"),
          broker("127.0.0.1"),
          conn_opts(""),
          max_frame_size(""),
          address(""),
          
          timeout(0),
          count(1),
          duration(0),
          duration_mode("after-send"),
          
          close_sleep(0),

          msg_id(""),
          msg_replyto(""),
          msg_subject(""),
          msg_durable(""),
          msg_ttl(-1),
          msg_priority(-1),
          msg_correlation_id(""),
          msg_user_id(""),
          msg_content_type(""),
          msg_content(""),
          msg_content_from_file(""),

          tx_size(0),
          tx_action("commit"),
          tx_el_action("None"),
          
          sync_mode("session"),
          capacity(-1),
          
          log_msgs(""),
          log_stats(""),
          log_lib("")
    {
        add("broker,b", broker, "url of broker to connect to");
        add("connection-options", conn_opts, "connection options string in the form {name1:value1, name2:value2}");
        
        std::stringstream sstm;
        sstm << "maximum frame size (" << MIN_MAX_FRAME_SIZE << " - " << UINT16_MAX << ", default: " << UINT16_MAX << ")";
        add("conn-max-frame-size", max_frame_size, sstm.str());
        
        add("address,a", address, "AMQP address");
        
        add("timeout,t", timeout, "exit after the specified time");
        add("count,c", count, "stop after count messages have been sent, zero disables");
        add("duration", duration, "message actions total duration (defines msg-rate together with count)");
        add("duration-mode", duration_mode, "specifies where to wait to achieve expected duration");

        add("close-sleep,s", close_sleep, "sleep before sender/receiver/session/connection.close()");

        
        add("msg-id", msg_id, "use the supplied id instead of generating one");
        add("msg-reply-to", msg_replyto, "specify reply-to address");
        add("msg-subject", msg_subject, "specify message subject");
        add("msg-property", msg_properties, "specify message property");
        add("msg-durable", msg_durable, "send durable messages yes/no");
        add("msg-ttl", msg_ttl, "message time-to-live (ms)");
        add("msg-priority", msg_priority, "message priority");
        add("msg-correlation-id", msg_correlation_id, "message correlation id");
        add("msg-user-id", msg_user_id, "message user id");
        add("msg-content-type", msg_content_type, "message content-type");

        add("msg-content", msg_content, "specify textual content");
        add("msg-content-from-file", msg_content_from_file, "specify file name to load content from");


        add("msg-content-list-item,L", list_entries, "specify entry for list content");
        add("msg-content-map-item,M", map_entries, "specify entry for map content");
        
        add("content-type", content_type, "specify how to treat/recast the msg content");



        add("tx-size", tx_size, "transactional mode: batch message count size (negative skips tx-action before exit)");
        add("tx-action", tx_action, "transactional action at the end of tx batch");
        add("tx-endloop-action", tx_el_action, "transactional action at the end of processing loop");

        add("sync-mode", sync_mode, "synchronization mode: none/session/action");
        add("capacity", capacity, "set sender's capacity");

        
        add("log-msgs", log_msgs, "message[s] reporting style (dict|body|upstream|none)");
        add("log-stats", log_stats, "report various statistic/debug information");
        add("log-lib", log_lib, "client logging library level");

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
        for (string_vector::const_iterator i = msg_properties.begin();
             i != msg_properties.end(); ++i) {
            setProperty(message, *i);
        }
    }

    void setListEntries(Variant::List& content) const
    {
        for (string_vector::const_iterator i = list_entries.begin();
             i != list_entries.end(); ++i) {
            std::string value = *i;
            if (value[0] == '~') {
                content.push_back(Variant().parse(value.erase(0, 1)));
            } else {
                 Variant content_string = Variant(value);
                 content_string.setEncoding("utf8");
                 content.push_back(content_string);
            }
        }
    }

    void setMapEntries(Variant::Map& content) const
    {
        for (string_vector::const_iterator i = map_entries.begin();
             i != map_entries.end(); ++i) {
            std::string name;
            std::string value;
            std::string separator;
            if (nameval(*i, name, value, separator)) {
                if (separator == "~") {
                  content[name] = Variant().parse(value);
                } else {
                  content[name] = Variant(value);
                  content[name].setEncoding("utf8");
                }
            } else {
                content[name] = Variant();
            }
        }
    }

    void transformValues()
    {
         std::transform(msg_durable.begin(), msg_durable.end(), msg_durable.begin(), ::tolower);
         std::transform(tx_el_action.begin(), tx_el_action.end(), tx_el_action.begin(), ::tolower);
         std::transform(tx_action.begin(), tx_action.end(), tx_action.begin(), ::tolower);
         if (tx_size < 0)
             tx_size = -tx_size;
    }

    void validate()
    {
        if (max_frame_size != "") {
            unsigned long max_frame_size_opt = strtoul(max_frame_size.c_str(), NULL, 10);
            
            if ((max_frame_size_opt > UINT16_MAX || max_frame_size_opt < MIN_MAX_FRAME_SIZE)) {
                std::stringstream sstm;
                sstm << "Maximum frame size " << max_frame_size << " is out of range (" << MIN_MAX_FRAME_SIZE << " - " << UINT16_MAX << ")";

                error(sstm.str());
            
                exit(EXIT_FAILURE);
            }
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
    
    if (options.parse(argc, argv)) {
        options.validate();

        // init timestamping
        ptsdata = ts_init((double *) &tsdata, options.log_stats);
        options.transformValues();
        bool msg_content_fmt = ( (options.msg_content.find('%') != std::string::npos) && \
                                 (options.msg_content.find('d') != std::string::npos) && \
                                 (options.msg_content.find('d', options.msg_content.find('%')) != std::string::npos) );

        ts_snap_store(ptsdata, 'B', options.log_stats);
        Connection connection(options.broker, options.conn_opts);

        if (options.max_frame_size != "") {
            connection.setOption("max_frame_size", options.max_frame_size);
        }
        
        try {
            connection.open();
            ts_snap_store(ptsdata, 'C', options.log_stats);
            Session session;
            if( (options.tx_size != 0) || (options.tx_el_action != "none") )
                session = connection.createTransactionalSession();
            else
                session = connection.createSession();
            ts_snap_store(ptsdata, 'D', options.log_stats);
            Sender sender = session.createSender(options.address);
            // set sender's capacity (if defined as > -1)
            if (options.capacity > -1)
                sender.setCapacity(options.capacity);

            Message message;
            options.setProperties(message);
            // assignment of message content/body
            // assignment priority map else list else string else from-file
            if (options.map_entries.size()) {
                // assignment of message body from the map entries (-M)
                Variant::Map content;
                options.setMapEntries(content);
                message.setContentType("amqp/map");
#ifdef NGETCONTENTOBJECT
                encode(content, message);
#else
                message.getContentObject() = content;
#endif
            } else if (options.list_entries.size()) {
                // assignment of message body from the list entries (-L)
                Variant::List content;
                options.setListEntries(content);
                message.setContentType("amqp/list");
#ifdef NGETCONTENTOBJECT
                encode(content, message);
#else
                message.getContentObject() = content;
#endif
            } else if (options.msg_content.size()) {
                // assignment of message body from the --msg-content
#ifdef NGETCONTENTOBJECT
                message.setContent(options.msg_content);
#else
                message.getContentObject() = options.msg_content;
                message.getContentObject().setEncoding("utf8");
#endif
                message.setContentType("text/plain");
            } else if (options.msg_content_from_file.size()) {
              // assignment of message body from the --msg-content-from-file
              std::string text_buffer;
              std::ifstream content_file;
              // enables throwing of exceptions for used ifstream
              std::ios_base::iostate exceptionMask = content_file.exceptions() | std::ios::failbit;
              content_file.exceptions(exceptionMask);
              try {
                content_file.open(options.msg_content_from_file.c_str());
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
                throw std::runtime_error("Error opening file: " + options.msg_content_from_file);
              }

              // set content
              //
                message.setContentType("text/plain");
            }

            // set message content type (if explicitly specified)
            if (!options.msg_content_type.empty())
              message.setContentType(options.msg_content_type);

            // set message subject
            if (!options.msg_subject.empty()) {
               message.setSubject(options.msg_subject);
            }
            /* Durability settings */
            // default = "" supports for 3 state durability
            if ( (options.msg_durable == "yes") || (options.msg_durable == "true") ) {
              message.setDurable(true);
            }
            else if ( (options.msg_durable == "no") || (options.msg_durable == "false")  ) {
                message.setDurable(false);
            }

            if (options.msg_ttl > -1)
                message.setTtl(Duration(options.msg_ttl));
            if (options.msg_priority > -1)
                message.setPriority(options.msg_priority);
            bool tx_open_batch_flag = false;
            std::time_t start = std::time(0);
            double ts = get_time();
            ts_snap_store(ptsdata, 'E', options.log_stats);
            for (int count = 0;
                (count < options.count || options.count == 0) &&
                (options.timeout == 0 || std::difftime(std::time(0), start) < options.timeout);
                count++) {

                // set message headers and properties
                if (!options.msg_replyto.empty()) {
                   message.setReplyTo(Address(options.msg_replyto));
                }

                if (options.msg_id != "")
                  message.setMessageId(options.msg_id);
                if (options.msg_correlation_id != "")
                  message.setCorrelationId(options.msg_correlation_id);
                if (options.msg_user_id != "")
                  message.setUserId(options.msg_user_id);

                // assign formatted message content
                if (msg_content_fmt) {
#ifdef NGETCONTENTOBJECT
                 message.setContent( boost::str(boost::format(options.msg_content) % count) );
#else
                 message.getContentObject() = boost::str(boost::format(options.msg_content) % count);
                 message.getContentObject().setEncoding("utf8");
#endif
                }
                if (ptsdata != NULL) {
                   message.getProperties()["ts"] = get_time();
                }

                // define message rate --count + --duration
                if( (options.duration > 0) && (options.duration_mode == "before-send") )
                    sleep4next(ts, options.count, options.duration, count+1);

                sender.send(message);

                if (options.log_stats.find("endpoints") != std::string::npos) {
                  Variant::Map stats = getSenderStats(sender);
		  
		  printStatistics(message, stats);
                }

                if (options.sync_mode == "action")
                    // call sync after every action (send())
                    session.sync();

                if (options.log_msgs == "body") {
                    std::cout << message.getContent() << std::endl;
                } else if (options.log_msgs == "dict") {
                    printMessageDict(message);
                }

                // define message rate --count + --duration
                if( (options.duration > 0) && (options.duration_mode == "after-send") )
                    sleep4next(ts, options.count, options.duration, count+1);

                if (options.tx_size != 0) {
                    // transactions enabled
                    if (((count+1) % options.tx_size) == 0) {
                        // end of transactional batch
                        if (options.tx_action == "rollback") {
                            session.rollback();
                            tx_open_batch_flag = false;
                        }
                        else {
                            // As described in the ambiguity document, defaults 
                            // to commit action
                            session.commit();
                            tx_open_batch_flag = false;
                        }
                    } else {
                        tx_open_batch_flag = true;
                    }
                }

                // define message rate --count + --duration
                if( (options.duration > 0) && (options.duration_mode == "after-send-tx-action") )
                    sleep4next(ts, options.count, options.duration, count+1);
            }
            ts_snap_store(ptsdata, 'F', options.log_stats);
            // end of message stream, tx-endloop-action if enabled
            if ( (tx_open_batch_flag == true) || (options.tx_el_action != "none") ) {
                if (options.tx_el_action == "commit")
                    session.commit();
                else if (options.tx_el_action == "rollback")
                    session.rollback();
            }
            if (options.sync_mode == "session")
                // call sync before session.close()
                session.sync();

            // sender/receiver/session/connection.close() sleep
            if (options.close_sleep)
                msleep(options.close_sleep * 1000);

            if (options.log_stats.find("endpoints") != std::string::npos) {
                Variant::Map stats = getSenderStats(sender);

		printStatistics(message, stats);
            }

            session.close();
            connection.close();
            ts_snap_store(ptsdata, 'G', options.log_stats);
            // report timestamping before success exit
            if (ptsdata != NULL)
                std::cout << "STATS " << ts_report(ptsdata, options.count,
                                                   message.getContent().size(), 0)
                      << std::endl;
            return 0;
        } catch(const std::exception& error) {
            std::cerr << error.what() << std::endl;
            connection.close();
            ts_snap_store(ptsdata, 'G', options.log_stats);
        }
    }
    // report timestamping before failure exit
    if (ptsdata != NULL)
        std::cout << "STATS " << ts_report(ptsdata, options.count,
                                           options.msg_content.size(), 1)
                  << std::endl;
    return 1;
}

// eof
