/*
 * SendingClient.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: opiske
 */

#include "SendingClient.h"

using proton::message;
using proton::container;


namespace dtests {
namespace proton {
namespace reactor {

using namespace std;
using dtests::common::UriParser;
using dtests::proton::common::ModernClient;

SendingClient::SendingClient()
: super()

{

}

SendingClient::~SendingClient()
{

}

void SendingClient::setMessageOptions(const OptionsSetter &setter,
        message &msg) const
{
    /*
     * Why the cast, you might ask? Because reply_to is overloaded, 
     * and the compiler is not smart enough to pick the correct one. 
     * Therefore we cast it (but not really) so that the compiler knows 
     * we are talking about reactor_type member function pointer (which is
     * defined in the header file analogous to this one.
     */
    setter.set("msg-reply-to", &msg,
            static_cast<string_setter> (&message::reply_to));

    setter.set("msg-to", &msg,
            static_cast<string_setter> (&message::to));

    setter.set("msg-correlation-id", &msg,
            static_cast<id_setter> (&message::correlation_id));

    setter.set("msg-content-type", &msg,
            static_cast<string_setter> (&message::content_type));

    setter.set("msg-subject", &msg,
            static_cast<string_setter> (&message::subject));

    setter.set("msg-id", &msg, static_cast<id_setter> (&message::id));

#ifdef ENABLE_IMPLICIT_CONVERSIONS
    duration def = duration(0);
    setter.setNumber("msg-ttl", &msg,
            static_cast<duration_setter> (&message::ttl), def);
#endif

    setter.set("msg-user-id", &msg,
            static_cast<string_setter> (&message::user));

    setter.setNumber("msg-priority", &msg,
            static_cast<uint_setter> (&message::priority));

    setter.setBoolean("msg-durable", &msg,
            static_cast<boolean_setter> (&message::durable));

    setter.set("msg-group-id", &msg,
            static_cast<string_setter> (&message::group_id));

    setter.set("reply-to-group-id", &msg,
            static_cast<string_setter> (&message::reply_to_group_id));
/*
 * This is replaced by setMessageProperties
 *
#ifdef REACTOR_PROPERTY_MAP_USES_STL
    message::property_map &properties = msg.properties();
    
    setter.setMap("msg-property", properties);
#endif // REACTOR_PROPERTY_MAP_USES_STL
 */
}

bool SendingClient::nameVal(const string &in, string &name, string &value, string &separator) const
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

void SendingClient::setMessageProperty(message *msg, const string &property) const
{
    string name;
    string val;
    string separator;
    string temp;

    if (nameVal(property, name, val, separator)) {
        if (separator == "~") {
          temp.resize(val.size());
          
          std::transform(val.begin(), val.end(), temp.begin(), ::tolower);

          if (temp == "true") {
            // true
            msg->properties().put(name, true);
          } else if (temp == "false") {
            // false
            msg->properties().put(name, false);
          } else if (val.find(".") != std::string::npos || val.find("e") != std::string::npos || val.find("E") != std::string::npos) {
            // maybe double
            try {
                // double
                msg->properties().put(name, atof(val.c_str()));
            } catch (exception& e) {
                // string
                msg->properties().put(name, val);
            }
          } else {
            // long
            msg->properties().put(name, atol(val.c_str()));
          }
          // msg->properties().get(name).setEncoding("utf8");
        } else {
          msg->properties().put(name, val);
          // msg->properties().get(name).setEncoding("utf8");
        }
    } else {
        msg->properties().put(name, val);
    }
}

void SendingClient::setMessageListItem(message *msg, const string &property, std::list<value> &messageList) const
{
    string name;
    string val;
    string separator;
    string temp;

    if (nameVal(property, name, val, separator)) {
        if (separator == "~") {
          temp.resize(val.size());
          
          std::transform(val.begin(), val.end(), temp.begin(), ::tolower);

          if (temp == "true") {
            // true
            messageList.push_back(true);
          } else if (temp == "false") {
            // false
            messageList.push_back(false);
          } else if (val.find(".") != std::string::npos || val.find("e") != std::string::npos || val.find("E") != std::string::npos) {
            // maybe double
            try {
                // double
                messageList.push_back(atof(val.c_str()));
            } catch (exception& e) {
                // string
                messageList.push_back(val);
            }
          } else if (val.find("~") != std::string::npos) {
            // string
            messageList.push_back(val);
          } else {
            // long
            messageList.push_back(atol(val.c_str()));
          }
          // msg->properties().get(name).setEncoding("utf8");
        } else {
          messageList.push_back(name);
          // msg->properties().get(name).setEncoding("utf8");
        }
    } else {
        messageList.push_back(name);
    }
}

void SendingClient::setMessageMapItem(message *msg, const string &property, std::map<std::string, value> &messageMap) const
{
    string name;
    string val;
    string separator;
    string temp;

    if (nameVal(property, name, val, separator)) {
        if (separator == "~") {
          temp.resize(val.size());
          
          std::transform(val.begin(), val.end(), temp.begin(), ::tolower);

          if (temp == "true") {
            // true
            messageMap[name] = true;
          } else if (temp == "false") {
            // false
            messageMap[name] = false;
          } else if (val.find(".") != std::string::npos || val.find("e") != std::string::npos || val.find("E") != std::string::npos) {
            // maybe double
            try {
                // double
                messageMap[name] = atof(val.c_str());
            } catch (exception& e) {
                // string
                messageMap[name] = val;
            }
          } else {
            // long
            messageMap[name] = atol(val.c_str());
          }
          // msg->properties().get(name).setEncoding("utf8");
        } else {
          messageMap[name] = val;
          // msg->properties().get(name).setEncoding("utf8");
        }
    } else {
        messageMap[name] = val;
    }
}

void SendingClient::setMessageProperties(StringAppendCallback &callbackProperty, message *msg) const
{
    vector<string> properties = callbackProperty.getStrings();

    for (vector<string>::iterator it = properties.begin(); it != properties.end(); ++it) {
        setMessageProperty(msg, *it);
    }
}

void SendingClient::setMessageList(StringAppendCallback &callbackList, message *msg) const
{
    vector<string> list = callbackList.getStrings();

    std::list<value> messageList;

    for (vector<string>::iterator it = list.begin(); it != list.end(); ++it) {
        setMessageListItem(msg, *it, messageList);
    }

    msg->body() = messageList;
}

void SendingClient::setMessageMap(StringAppendCallback &callbackMap, message *msg) const
{
    vector<string> map = callbackMap.getStrings();

    std::map<std::string, value> messageMap;

    for (vector<string>::iterator it = map.begin(); it != map.end(); ++it) {
        setMessageMapItem(msg, *it, messageMap);
    }

    msg->body() = messageMap;
}

void SendingClient::setMessageText(string content, message *msg) const {
    msg->body(content);
}

int SendingClient::run(int argc, char **argv) const
{
    const string usage = "usage: %prog [OPTION]... DIR [FILE]...";
    const string version = "1.0";
    const string desc = "C/C++ AMQ reactive API sender client for Qpid Proton";

    SenderOptionsParser parser = SenderOptionsParser();
    UriParser uri_parser = UriParser();

    /**
     * WARNING: do not reassign the result of chainned calls to usage/version/etc
     *  because it causes the code to segfault. For example, this crashes:
     *
     *  ControlOptions parser = ControlOptions().usage(usage)
     */
    parser.usage(usage).version(version).description(desc);

    optparse::Values options = parser.parse_args(argc, argv);

    parser.validate(options);

    setLogLevel(options);

    // Temporary variable for address, will search for prefix
    string tempAddress = options["broker-url"];
    // Variable for final address
    string address;
    // Variable for recognition of topic
    bool is_topic = false;
    // Search for prefix
    std::size_t prefix_index = tempAddress.find("topic://");
    // If prefix is present
    if (prefix_index != std::string::npos) {
        // Delete prefix
        address = tempAddress.replace(prefix_index, 8, "");
        // Set that it will be topic
        is_topic = true;
    // If prefix is NOT present
    } else {
        // Use full address
        address = tempAddress;
    }

    uri_parser.parse(options["broker-url"]);

    string user = "";
    if (options.is_set("user")) {
        user = options["user"];
    } else {
        user = uri_parser.getUser();
    }
    
    string password = "";
    if (options.is_set("password")) {
        password = options["password"];
    } else {
        password = uri_parser.getPassword();
    }
    
    string sasl_mechanisms = "";
    if (options.is_set("sasl-mechanisms")) {
        sasl_mechanisms = options["sasl-mechanisms"];
    } else if (user != "" && password != "") {
        sasl_mechanisms = "PLAIN";
    } else {
        sasl_mechanisms = "ANONYMOUS";
    }
    
    bool conn_reconnect_custom = false;

    string conn_reconnect = "true";
    if (options.is_set("conn-reconnect")) {
        conn_reconnect = options["conn-reconnect"];
        std::transform(conn_reconnect.begin(), conn_reconnect.end(), conn_reconnect.begin(), ::tolower);
    }

    int32_t conn_reconnect_interval = 10;
    if (options.is_set("conn-reconnect-interval")) {
        conn_reconnect_interval = std::strtol(options["conn-reconnect-interval"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    int32_t conn_reconnect_limit = 0;
    if (options.is_set("conn-reconnect-limit")) {
        conn_reconnect_limit = std::strtol(options["conn-reconnect-limit"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    int32_t conn_reconnect_timeout = duration::FOREVER.milliseconds();
    if (options.is_set("conn-reconnect-timeout")) {
        conn_reconnect_timeout = std::strtol(options["conn-reconnect-timeout"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    uint32_t conn_reconnect_first = 0;
    if (options.is_set("conn-reconnect-first")) {
        conn_reconnect_first = std::strtoul(options["conn-reconnect-first"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    uint32_t conn_reconnect_increment = 100;
    if (options.is_set("conn-reconnect-increment")) {
        conn_reconnect_increment = std::strtoul(options["conn-reconnect-increment"].c_str(), NULL, 10);

        conn_reconnect_custom = true;
    }

    bool conn_reconnect_doubling = true;
    if (options.is_set("conn-reconnect-doubling")) {
        if (options["conn-reconnect-doubling"] == "false") {
            conn_reconnect_doubling = false;
        }

        conn_reconnect_custom = true;
    }

    uint32_t conn_heartbeat = 0;
    if (options.is_set("conn-heartbeat")) {
        conn_heartbeat = std::strtoul(options["conn-heartbeat"].c_str(), NULL, 10);
    }

    uint32_t max_frame_size = -1;
    if (options.is_set("conn-max-frame-size")) {
        max_frame_size = std::strtoul(options["conn-max-frame-size"].c_str(), NULL, 10);
    }

    OptionsSetter setter = OptionsSetter(options);

    int timeout = 1;
    if (options.is_set("timeout")) {
        timeout = static_cast<int> (options.get("timeout"));
    }

    string log_msgs = "";
    if (options.is_set("log-msgs")) {
        log_msgs = options["log-msgs"];
    }

    int duration = 0;
    if (options.is_set("duration")) {
        duration = static_cast<int> (options.get("duration"));
    }

    string duration_mode = "after-send";
    if (options.is_set("duration-mode")) {
        duration_mode = options["duration-mode"];
    }

    message msg;

    setMessageOptions(setter, msg);
    setMessageProperties(parser.callbackProperty, &msg);

    if (parser.callbackList.str.length() > 0) {
        // List
        setMessageList(parser.callbackList, &msg);
    } else if (parser.callbackMap.str.length() > 0) {
        // Map
        setMessageMap(parser.callbackMap, &msg);
    } else {
        // Text
        setMessageText(options["msg-content"], &msg);
    }

   
/*
 * Note 1: this is a left-over from setMessageOptions. Since I don't want to 
 * change the method signature there, I check again here and set the remaining
 * option that cannot be done implicitly above.
 * 
 * Note 2: this is a hack for GCC ~4.4.7 on i686. 
 */    
#ifndef ENABLE_IMPLICIT_CONVERSIONS
    long value =  options.get("msg-ttl");
    
    msg.ttl(duration(value));
#endif
    if (options.is_set("msg-group-seq")) {
        msg.group_sequence(std::strtol(options["msg-group-seq"].c_str(), NULL, 10));
    }

    SenderHandler handler = SenderHandler(
        address,
        parser.callbackFailoverUrl.getStrings(),
        is_topic,
        user,
        password,
        sasl_mechanisms,
        timeout,
        duration,
        duration_mode,
        conn_reconnect,
        conn_reconnect_interval,
        conn_reconnect_limit,
        conn_reconnect_timeout,
        conn_reconnect_first,
        conn_reconnect_increment,
        conn_reconnect_doubling,
        conn_reconnect_custom,
        conn_heartbeat,
        max_frame_size,
        log_msgs
    );

    handler.setMessage(msg);
    
    int count = 1;
    if (options.is_set("count")) {
        count = static_cast<int> (options.get("count"));
    }
    handler.setCount(count);
    
    try {
        container(handler).run();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }

    return 1;
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
