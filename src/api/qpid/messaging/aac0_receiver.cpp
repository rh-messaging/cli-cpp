// aac0_receiver client

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

using namespace qpid::messaging;
using namespace qpid::types;

struct Options : OptionParser
{
    std::string broker;
    std::string conn_opts;
    std::string address;
    
    int timeout;
    int count;
    int duration;
    std::string duration_mode;
    
    int close_sleep;
    
    std::string action;

    int tx_size;
    std::string tx_action;
    std::string tx_le_action;
    
    std::string sync_mode;
    int capacity;
    
    std::string log_msgs;
    std::string log_stats;
    std::string log_lib;

    Options()
        : OptionParser("Usage: aac0_receiver [OPTIONS]", "Receive messages from the specified address"),
          broker("127.0.0.1"),
          conn_opts(""),
          address(""),
          
          timeout(0),
          count(0),
          duration(0),
          duration_mode("after-receive"),
          
          close_sleep(0),
          
          action(""),

          tx_size(0),
          tx_action("commit"),
          tx_le_action("commit"),
          
          sync_mode("none"),
          capacity(-1),
          
          log_msgs("None"),
          log_stats(""),
          log_lib("")
    {
        add("broker,b", broker, "url of broker to connect to");
        add("connection-options", conn_opts, "connection options string in the form {name1:value1, name2:value2}");
        add("address,a", address, "AMQP address");
        
        add("timeout,t", timeout, "timeout in seconds to wait before exiting");
        add("count,c", count, "number of messages to read before exiting");
        add("duration", duration, "message actions total duration (defines msg-rate together with count)");
        add("duration-mode", duration_mode, "specifies where to wait to achieve expected duration");
        
        add("close-sleep,s", close_sleep, "sleep before sender/receiver/session/connection.close()");
        
        add("action", action, "action on acquired message");

        add("tx-size", tx_size, "transactional mode: batch message count size");
        add("tx-action", tx_action, "transactional action at the end of tx batch");
        add("tx-loopend-action", tx_le_action, "transactional action at the end of processing loop");

        add("sync-mode", sync_mode, "synchronization mode: none/session/action");
        add("capacity", capacity, "set receiver's capacity");

        add("log-msgs", log_msgs, "message[s] reporting style (dict|body|upstream|none)");
        add("log-stats", log_stats, "report various statistic/debug information");
        // TODO: --log-lib wiring
        add("log-lib", log_lib, "client logging library level");
    }

    Duration getTimeout()
    {
        if (timeout == -1) return Duration::FOREVER;
        else return timeout*Duration::SECOND;
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
    Formatter formatter;
    if (options.parse(argc, argv)) {

        // init timestamping
        ptsdata = ts_init((double *) &tsdata, options.log_stats);

        ts_snap_store(ptsdata, 'B', options.log_stats);
        Connection connection(options.broker, options.conn_opts);
        try {
            connection.open();
            ts_snap_store(ptsdata, 'C', options.log_stats);
            Session session;
            if (options.tx_size != 0)
                session = connection.createTransactionalSession();
            else
                session = connection.createSession();
            ts_snap_store(ptsdata, 'D', options.log_stats);
            Receiver receiver = session.createReceiver(options.address);
            // set receiver's capacity (if defined as > -1)
            if (options.capacity > -1)
                receiver.setCapacity(options.capacity);

            Duration timeout = options.getTimeout();
            int count = options.count;
            Message message;
            long int message_size;
            int i = 0;
            bool tx_batch_flag = (options.tx_size != 0);
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
                    formatter.printMessageAsDict(message);
                }

                // define message rate --count + --duration
                if( (options.duration > 0) && (options.duration_mode == "after-receive") )
                    sleep4next(ts, options.count, options.duration, i+1);

                if (options.action == "reject")
                    session.reject(message);
                else if (options.action == "release")
                    session.release(message);
                else if (options.action != "noack")
                    session.acknowledge(message);

                // define message rate --count + --duration
                if( (options.duration > 0) && (options.duration_mode == "after-receive-action") )
                    sleep4next(ts, options.count, options.duration, i+1);

                if(options.tx_size != 0) {
                    // transactions enabled
                    if( (((i+1) % abs(options.tx_size)) == 0) ) {
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
                if( (options.duration > 0) && (options.duration_mode == "after-receive-action-tx-action") )
                    sleep4next(ts, options.count, options.duration, i+1);

                // get message size
                if (i == 0)
                    message_size = message.getContent().size();

                if ((++i == count) && count)
                    break;
            }
            ts_snap_store(ptsdata, 'F', options.log_stats);

            // end of message stream, tx-loopend-action if enabled
            if (tx_batch_flag == true) {
                if (options.tx_le_action == "commit")
                    session.commit();
                else if (options.tx_le_action == "rollback")
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
