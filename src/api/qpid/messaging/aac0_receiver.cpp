// aac0_receiver client

// TODO: --log-lib wiring
// TODO: --duration-mode=before-receive
// TODO: --duration <float>

#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include <qpid/messaging/Address.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <typeinfo>

#include "OptionParser.h"
#include "Utils.h"
#include "formatter/FormatUtil.h"

#define MIN_MAX_FRAME_SIZE 4096

using namespace qpid::messaging;
using namespace qpid::types;

using dtests::qpid::messaging::printMessageDict;
using dtests::qpid::messaging::printMessageInterop;
using dtests::qpid::messaging::printStatistics;


struct Options : OptionParser
{
    std::string broker;
    std::string conn_opts;
    std::string max_frame_size;
    std::string address;
    std::string selector;
    
    int timeout;
    int count;
    int duration;
    std::string duration_mode;
    
    int close_sleep;

    bool process_replyto;
    
    std::string action;
    std::string action_le;
    int action_size;

    int tx_size;
    std::string tx_action;
    std::string tx_el_action;
    
    std::string sync_mode;
    int capacity;
    
    std::string log_msgs;
    std::string log_stats;
    std::string log_lib;

    Options()
        : OptionParser("Usage: aac0_receiver [OPTIONS]", "Receive messages from the specified address"),
          broker("127.0.0.1"),
          conn_opts(""),
          max_frame_size(""),
          address(""),
          selector(""),
          
          timeout(0),
          count(0),
          duration(0),
          duration_mode("after-receive"),
          
          close_sleep(0),

          process_replyto(false),
          
          action("ack"),
          action_le(""),
          action_size(1),

          tx_size(0),
          tx_action("commit"),
          tx_el_action("None"),
          
          sync_mode("none"),
          capacity(-1),
          
          log_msgs("None"),
          log_stats(""),
          log_lib("")
    {
        add("broker,b", broker, "url of broker to connect to");
        add("connection-options", conn_opts, "connection options string in the form {name1:value1, name2:value2}");

        std::stringstream sstm;
        sstm << "maximum frame size (" << MIN_MAX_FRAME_SIZE << " - " << UINT16_MAX << ", default: " << UINT16_MAX << ")";
        add("conn-max-frame-size", max_frame_size, sstm.str());

        add("address,a", address, "AMQP address");
        add("msg-selector", selector, "message server side selector. Please note that Qpid C++ Messaging API supply selector via address, this will just substitue \"'%MSG_SELECTOR%'\" (with singlequotes) string in the address field.");
        
        add("timeout,t", timeout, "timeout in seconds to wait before exiting");
        add("count,c", count, "number of messages to read before exiting");
        add("duration", duration, "message actions total duration (defines msg-rate together with count)");
        add("duration-mode", duration_mode, "specifies where to wait to achieve expected duration");
        
        add("close-sleep,s", close_sleep, "sleep before sender/receiver/session/connection.close()");
        
        add("process-reply-to", process_replyto, "send the message with replyto header available");

        add("action", action, "action on acquired message");
        add("action-loopend", action_le, "action on acquired message at the end of processing loop");
        add("action-size", action_size, "frequency of actions on messages (every N message[s])");

        add("tx-size", tx_size, "transactional mode: batch message count size");
        add("tx-action", tx_action, "transactional action at the end of tx batch");
        add("tx-endloop-action", tx_el_action, "transactional action at the end of processing loop");

        add("sync-mode", sync_mode, "synchronization mode: none/session/action");
        add("capacity", capacity, "set receiver's capacity");

        add("log-msgs", log_msgs, "message[s] reporting style (dict|interop|body)");
        add("log-stats", log_stats, "report various statistic/debug information");
        add("log-lib", log_lib, "client logging library level");
    }

    Duration getTimeout()
    {
        if (timeout == -1) return Duration::FOREVER;
        else return timeout*Duration::SECOND;
    }

    void transformValues()
    {
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

            // Remove prefix 'topic://' when FQQN is specified
            std::string fixedAddress;
            std::size_t topic_prefix_index = options.address.find("topic://");
            std::size_t double_colon_index = options.address.find("::");
            if (topic_prefix_index != std::string::npos && double_colon_index != std::string::npos) {
                fixedAddress = options.address.replace(topic_prefix_index, 8, "");
            } else {
                fixedAddress = options.address;
            }

            // selector substitution
            if (options.selector != "") {
              std::string pattern = "'%MSG_SELECTOR%'";
              size_t xpos = fixedAddress.find(pattern);
              if (xpos != std::string::npos) {
                std::string tmp = "\"" + options.selector + "\"";
                fixedAddress.replace(xpos, pattern.length(), tmp);
              }
            }

            Receiver receiver = session.createReceiver(fixedAddress);

            Sender sender = NULL;
            // set receiver's capacity (if defined as > -1)
            if (options.capacity > -1)
                receiver.setCapacity(options.capacity);

            Duration timeout = options.getTimeout();
            int count = options.count;
            Message message;
            long int message_size;
            int i = 0;
            bool tx_open_batch_flag = false;
            double ts = get_time();

            ts_snap_store(ptsdata, 'E', options.log_stats);

            while (receiver.fetch(message, timeout)) {

                if( (ptsdata != NULL) && 
                    (message.getProperties()["ts"].getType() != VAR_VOID) ) {
                    ts_snap_store(ptsdata, 'L', options.log_stats,
                                  (double)message.getProperties()["ts"]);
                }

                if (options.sync_mode == "action")
                    // call sync after every action (fetch())
                    session.sync();

                if (options.log_msgs == "body") {
                    std::cout << message.getContent() << std::endl;
                } else if (options.log_msgs == "dict") {
                    printMessageDict(message);
                } else if (options.log_msgs == "interop") {
                    printMessageInterop(message);
                }

                // define message rate --count + --duration
                if( (options.duration > 0) && (options.duration_mode == "after-receive") )
                    sleep4next(ts, options.count, options.duration, i+1);

                // action on acquired message[s]
                if (options.action.find("ack") == 0) {
                    if (options.action_size == 1) {
                        // actions on every message
                        session.acknowledge(message);
                    } else {
                        // actions on message block
                        if( (((i+1) % abs(options.action_size)) == 0) )
                            session.acknowledgeUpTo(message);
                    }
                } else if (options.action == "reject")
                    session.reject(message);
                else if (options.action == "release")
                    session.release(message);

                // define message rate --count + --duration
                if( (options.duration > 0) && (options.duration_mode == "after-receive-action") )
                    sleep4next(ts, options.count, options.duration, i+1);

                if(options.tx_size != 0) {
                    // transactions enabled
                    if( (((i+1) % options.tx_size) == 0) ) {
                        // end of transactional batch
                        if (options.tx_action == "rollback") {
                            session.rollback();
                            tx_open_batch_flag = false;
                        }
                        else {
                            session.commit();
                            tx_open_batch_flag = false;
                        }
                    } else {
                        tx_open_batch_flag = true;
                    }
                }

                // define message rate --count + --duration
                if( (options.duration > 0) && (options.duration_mode == "after-receive-action-tx-action") )
                    sleep4next(ts, options.count, options.duration, i+1);

                // process reply-to (if enabled)
                if( (options.process_replyto) && (!message.getReplyTo().getName().empty()) ) {
                    sender = session.createSender(message.getReplyTo().getName());
                    sender.send(message);
                }

                // get message size
                if (i == 0)
                    message_size = message.getContent().size();

                if ((++i == count) && count)
                    break;
            }
            ts_snap_store(ptsdata, 'F', options.log_stats);

            // end of message stream, action-eldloop if enabled
            // Note: at this point we do not have a message anymore
            //       and performed action is done on session without message instance
            //       qpid::messaging allows here just acknowledge
            if (!options.action_le.empty()) {
                if (options.action_le.find("ack") == 0)
                    session.acknowledge();
            }

            // end of message stream, tx-endloop-action if enabled
            if ( (tx_open_batch_flag == true) || (options.tx_el_action != "none") ) {
                if (options.tx_el_action == "commit")
                    session.commit();
                else if (options.tx_el_action == "rollback")
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
            ts_snap_store(ptsdata, 'G', options.log_stats);
            // report timestamping before success exit
            if (ptsdata != NULL)
                std::cout << "STATS " << ts_report(ptsdata, i, message_size, 0)
                          << std::endl;
            return 0;
        } catch(const std::exception& error) {
            std::cerr << error.what() << std::endl;
            connection.close();
            ts_snap_store(ptsdata, 'G', options.log_stats);
            // report timestamping before failure exit
            if (ptsdata != NULL)
                std::cout << "STATS " << ts_report(ptsdata, -1, -1, 1)
                          << std::endl;
        }
    }
    return 1;
}

// eof
