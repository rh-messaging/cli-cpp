
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include <sys/types.h>
#include <signal.h>

#include <boost/regex.hpp>
#include <boost/format.hpp>

#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include <qpid/types/Exception.h>

#include "ObjectCloser.h"
#include "Utils.h"

#define MIN_MAX_FRAME_SIZE 4096

using namespace std;

void printUsage() {
  cout <<
"aac0_connector [(-b|--broker) <broker-url>] [(-c|--count) <connection_amount>]\n\
           [(--connection-options) <conn_options>]\n\
           [(--conn-max-frame-size) <max_frame_size>] [(-a|--address) <address>]\n\
           [--obj-ctrl <object-ids>] [--sync-mode <smode>] [--close-sleep <dur>]\n\
\n\
(-b|--broker) <broker>\n\
  Specify broker to connect in format <IP/name>:<port>\n\
  > default: localhost:5672\n\
\n\
(-c|--count) <connection_count>\n\
  Specify how many connection connector try to create and open\n\
  > default: 1\n\
\n\
(--connection-options) <connection_options>\n\
  Optional connection options (authentication, reconnect, etc.)\n\
  e.g. --connection-options \"{ username:X, password: Y, sasl_mechanisms: PLAIN}\"\n\
  > default: none\n\
\n\
(--conn-max-frame-size) <max_frame_size>\n\
  Optional maximum frame size (" << MIN_MAX_FRAME_SIZE << " - " << UINT16_MAX << ")\n\
  > default: " << UINT16_MAX << "\n\
\n\
(-a|--address) <address>\n\
  Optional AMQP address used for address creation/deletion/check of existance\n\
  e.g. -a \"MYQUEUE; {create: always, delete: receiver}\"\n\
  > default: none\n\
\n\
(--obj-ctrl) <object-ids>\n\
  Optional creation object control based on <object-ids>\n\
  syntax C/E/S/R stands for Connection, sEssion, Sender, Receiver\n\
  e.g. --obj-ctrl \"CES\" for creation of Connection+sEssion+Sender\n\
  > default: \"C\" (address not given) \"CESR\" (address specified)\n\
\n\
(--sync-mode) <smode>\n\
  Optional action synchronization mode: none/session/action\n\
  syntax none  / session                / action    corresponds to \n\
         never / before session.close() / at every operation\n\
  e.g. --sync-mode \"session\"\n\
  > default: \"action\"\n\
\n\
--close-sleep <dur> \n\
  Opened objects will be held until duration passes by \n\
  Also the sessions if exists will be synced every T=1s \n\
" << endl;
  return;
}

/** ECODE TABLE
  * 0 - EOK
  * 1 - General Error
  * 2 - Bad Usage Error
  */


int main (int argc, char ** argv) {

  //signal(SIGINT,hold_mode_exit);

  int result = 0;

// settings defaults
  string broker = "localhost:5672";
  string address = "";
  string obj_ctrl = "_";
  string sync_mode = "action";
  string conn_options = "";
  string max_frame_size = "";
  int count = 1;
  int duration = 0;

// future reserved
  float wait_time = 1.0; // means perform some action every T=1s of waiting

  int conn_counter = 0;

  string arg;
  for (int i = 1; i < argc; i++) {
    arg.assign(argv[i]);

    if (arg.compare("-b") == 0 || arg.compare("--broker") == 0) {
      if (++i<argc) {
        broker.assign(argv[i]);
      } else {
        cerr << "no value specified for \"" << argv[i]<< "\"";
        return(2);
      }
    } else if (arg.compare("-c") == 0 || arg.compare("--count") == 0) {
      if (++i<argc) {
        istringstream is(argv[i]);
        is >> count;
      } else {
        cerr << "no value specified for \"" << argv[i]<< "\"";
        return(2);
      }
    } else if (arg.compare("--connection-options") == 0) {
      if (++i<argc) {
        conn_options.assign(argv[i]);
      } else {
        cerr << "no value specified for \"" << argv[i]<< "\"";
        return(2);
      }
    } else if (arg.compare("--conn-max-frame-size") == 0) {
      if (++i<argc) {
        unsigned long max_frame_size_opt = strtoul(argv[i], NULL, 10);
            
        if ((max_frame_size_opt > UINT16_MAX || max_frame_size_opt < MIN_MAX_FRAME_SIZE)) {
          cerr << "Error: Maximum frame size " << argv[i] << " is out of range (" << MIN_MAX_FRAME_SIZE << " - " << UINT16_MAX << ")" << endl;
          return(2);
        }

        max_frame_size.assign(argv[i]);
      } else {
        cerr << "no value specified for \"" << argv[i]<< "\"";
        return(2);
      }
    } else if (arg.compare("-a") == 0 || arg.compare("--address") == 0) {
      if (++i<argc) {
        address.assign(argv[i]);
      } else {
        cerr << "no value specified for \"" << argv[i]<< "\"";
        return(2);
      }
    } else if (arg.compare("--obj-ctrl") == 0) {
      if (++i<argc) {
        obj_ctrl.assign(argv[i]);
      } else {
        cerr << "no value specified for \"" << argv[i]<< "\"";
        return(2);
      }
    } else if (arg.compare("--sync-mode") == 0) {
      if (++i<argc) {
        sync_mode.assign(argv[i]);
      } else {
        cerr << "no value specified for \"" << argv[i]<< "\"";
        return(2);
      }
    } else if (arg.compare("-h") == 0 || arg.compare("--help") == 0) {
      printUsage();
      return(0);
    } else if (arg.compare("--duration") == 0) {
      if (++i<argc) {
        istringstream is(argv[i]);
        is >> duration;
      } else {
        cerr << "no value specified for \"" << argv[i]<< "\"";
        return(2);
      }
    } else {
        cerr << "unknown argument \"" << argv[i]<< "\"" << endl << endl;
        printUsage();
        return(2);
    }

  }
  // assign --obj-ctrl default value (if not given from command-line)
  if (obj_ctrl.compare("_") == 0)
  {
    if (address.length() > 0)
      obj_ctrl = "CESR";
    else
      obj_ctrl = "C";
  }

  // declare number of connections
  // ------------------------------------------------------------------------
  vector<qpid::messaging::Connection> con_list;
  vector<qpid::messaging::Session> ssn_list;
  vector<qpid::messaging::Sender> snd_list;
  vector<qpid::messaging::Receiver> rcv_list;

  if (obj_ctrl.find_first_of("CESR") != string::npos) {
    // request to create at least connection
  
    // connection part
    // ----------------------------------------------------------------------
    try {
      for (int i=0; i < count; i++) {
        if (conn_options.length() > 0)
        {
          qpid::messaging::Connection tmp_connection(broker, conn_options);
          tmp_connection.setOption("max_frame_size", max_frame_size);
          con_list.push_back(tmp_connection);
        }
        else
        {
          qpid::messaging::Connection tmp_connection(broker);
          con_list.push_back(tmp_connection);
        }
    }
  
      // open connections
      // ----------------------------------------------------------------------
      vector<qpid::messaging::Connection>::iterator con_list_it;
      for (con_list_it = con_list.begin(); con_list_it != con_list.end(); con_list_it++) {
  
        /*
        * since tunnel failure is fixed, it's no need for recconection,
        * there was no problem with clients
        */
        //(*con_list_it).setOption("reconnect_limit", "3");
        //(*con_list_it).setOption("reconnect_interval", "0.1");
        (*con_list_it).open();
        //(*con_list_it).close();
        conn_counter++;
      }
      //cout << "Opened connections: " << conn_counter << endl;
    }
    catch(const qpid::types::Exception& error) {
      //cout << "Opened connections: " << conn_counter << endl;
      cerr << error.what() << endl;
        result |= 1;
    }
    catch(const exception& error) {
      //cout << "Opened connections: " << conn_counter << endl;
      cerr << error.what() << endl;
        result |= 1;
    }
  
    // count number of opened connections
    // ------------------------------------------------------------------------
    int opened = 0;
    int not_opened = 0;
  
    vector<qpid::messaging::Connection>::iterator con_list_it;
    for (con_list_it = con_list.begin(); con_list_it != con_list.end(); con_list_it++) {
      if ((*con_list_it).isOpen()) {
        opened++;
      } else {
        not_opened++;
      }
    }
  
    // connection state status
    // ------------------------------------------------------------------------
    cout << opened << " " << not_opened << " " << count << endl;
  
  
    if (obj_ctrl.find_first_of("ESR") != string::npos) {
      // request to create at least session (connection needed)
      
      // session part for opened connections
      // --------------------------------------------------------------------
      try {
        vector<qpid::messaging::Connection>::iterator con_list_it;
        for (con_list_it = con_list.begin(); con_list_it != con_list.end(); con_list_it++) {
          // browse all connections
          if ((*con_list_it).isOpen()) {
            qpid::messaging::Session s = (*con_list_it).createSession();
            ssn_list.push_back(s);
            if (sync_mode.compare("action") == 0)
              s.sync();
          } else {
            ssn_list.push_back(NULL);
          }
        }
      }
      catch(const qpid::types::Exception& error) {
        //cout << "Opened connections: " << conn_counter << endl;
        cerr << error.what() << endl;
        result |= 1;
        for (unsigned int fail_indx = ssn_list.size(); fail_indx < con_list.size(); fail_indx++) {
          ssn_list.push_back(NULL);
        }
      }
    
      if (address.length() > 0) {
        // further object require non-empty address
      
        if (obj_ctrl.find_first_of("S") != string::npos) {
          // request to create at least sender (connection+session needed)
        
          // sender part (if address is specified)
          // create senders for opened sessions
          // ------------------------------------------------------------------------
          try {
            for (unsigned int indx = 0; indx < ssn_list.size(); indx++) {
              // browse all sessions
              if (ssn_list[indx] != NULL) {
                snd_list.push_back(ssn_list[indx].createSender(str_fmt(address, indx)));
                if (sync_mode.compare("action") == 0)
                  ssn_list[indx].sync();
              } else {
                snd_list.push_back(NULL);
              }
            }
          }
          catch(const qpid::types::Exception& error) {
            //cout << "Opened connections: " << conn_counter << endl;
            cerr << error.what() << endl;
            result |= 1;
            for (unsigned int fail_indx = snd_list.size(); fail_indx < con_list.size(); fail_indx++) {
              snd_list.push_back(NULL);
            }
          }
        }
      
        if (obj_ctrl.find_first_of("R") != string::npos) {
          // request to create at least receiver (connection+session needed)
        
          // receiver part (if address is specified)
          // create receivers for opened sessions
          // ------------------------------------------------------------------
          try {
            for (unsigned int indx = 0; indx < ssn_list.size(); indx++) {
              // browse all sessions
              if (ssn_list[indx] != NULL) {
                rcv_list.push_back(ssn_list[indx].createReceiver(str_fmt(address, indx)));
                if (sync_mode.compare("action") == 0)
                  ssn_list[indx].sync();
              } else {
                rcv_list.push_back(NULL);
              }
            }
          }
          catch(const qpid::types::Exception& error) {
            //cout << "Opened connections: " << conn_counter << endl;
            cerr << error.what() << endl;
            result |= 1;
            for (unsigned int fail_indx = rcv_list.size(); fail_indx < con_list.size(); fail_indx++) {
              rcv_list.push_back(NULL);
            }
          }
        }
      }
    }
  
    // hold mode (infinite loop, signal has to come to exit the app)
    // ------------------------------------------------------------------------
    if (duration != 0) {
      double ts = get_time();
      int wait_rounds = (int)(duration / wait_time);
      for (int wait_round = 0; wait_round < wait_rounds; wait_round++) {
        // sleep before if duration is positive
        if (duration > 0) {
          sleep4next(ts, wait_rounds, duration, wait_round+1);
        }
        if (obj_ctrl.find_first_of("ESR") != string::npos) {
          try {
            for (unsigned int indx = 0; indx < ssn_list.size(); indx++) {
              // browse all sessions
              if (ssn_list[indx] != NULL)
                ssn_list[indx].sync();
            }
          }
          catch(const qpid::types::Exception& error) {
            cerr << error.what() << endl;
            result |= 1;
          }
        } // if (obj_ctrl.find_first_of("ESR") != string::npos)
        // sleep before if duration is neagtive
        if (duration < 0) {
          sleep4next(ts, wait_rounds, abs(duration), wait_round+1);
        }
      } //for (int wait_round = 0; wait_round < wait_rounds; wait_round++)
    } // if ((duration != 0) && (obj_ctrl.find_first_of("ESR") != string::npos))

    // --sync-mode=session sync() action (before) objects get closed
    if( (obj_ctrl.find_first_of("ESR") != string::npos) &&
        (sync_mode.compare("session") == 0) ) {
      try {
        for (unsigned int indx = 0; indx < ssn_list.size(); indx++) {
          // browse all sessions
          if (ssn_list[indx] != NULL)
            ssn_list[indx].sync();
        }
      }
      catch(const qpid::types::Exception& error) {
        cerr << error.what() << endl;
        result |= 1;
      }
    }

  }

  // close all opened senders, receivers, sessions, and connections
  // ------------------------------------------------------------------------
  result |= close_objects(snd_list);
  result |= close_objects(rcv_list);
  result |= close_objects(ssn_list);
  result |= close_objects(con_list);

  // exit code
  // ------------------------------------------------------------------------
  return result;
}

// eof
