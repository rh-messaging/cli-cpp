#include "FormatUtil.h"

namespace dtests {
namespace qpid {
namespace messaging {
	
using std::cout;

void printMessageUpstream(const Message &message, bool verbose) {
#ifdef ENABLE_MODERN
	QpidDecoder decoder = QpidDecoder(message); 
		
	std::ostringstream stream;
	UpstreamWriter writer = UpstreamWriter(&stream);
	
	UpstreamFormatter formatter = UpstreamFormatter();
	formatter.printMessage(&decoder, &writer);
	
	writer.endLine();
	std::cout << writer.toString();
	 
#else
	Formatter formatter;
	
	Message tmp = const_cast<Message &>(message);
	
	formatter.printMessage(tmp, verbose);
#endif // ENABLE_MODERN
}


void printMessageDict(const Message &message) {
#ifdef ENABLE_MODERN
	QpidDecoder decoder = QpidDecoder(message); 
	
	std::ostringstream stream;
	DictWriter writer = DictWriter(&stream);
	
	DictFormatter formatter = DictFormatter();
	formatter.printMessage(&decoder, &writer);
	
	writer.endLine();
	std::cout << writer.toString();
#else
	Formatter formatter;
	
	Message tmp = const_cast<Message &>(message);
	
	formatter.printMessageAsDict(tmp);
#endif // ENABLE_MODERN
}

std::string formatBool(const bool b) {
    return b ? "True" : "False";
}

std::string formatBool(const Variant b) {
    return b == "true" ? "True" : "False";
}

std::string formatString(const std::string s, bool remove_id_prefix) {
    if (s.empty()) {
        return std::string("None");
    } else {
        if (remove_id_prefix && (s.find("ID:") == 0)) {
            return std::string("'").append(s.substr(s.find(":")+1)).append("'");
        } else {
            return std::string("'").append(s).append("'");
        }
    }
}

std::string formatList(const std::list <Variant>l) {
    std::ostringstream oss;
    std::string::size_type index = 0;

    oss << "[";
    for (std::list<Variant>::const_iterator it = l.begin(); it != l.end(); it++) {
        oss << getValue(*it);
        index++;

        if (index < l.size()) {
            oss << ", ";
        }
    }
    oss << "]";

    return oss.str();
}

std::string formatMap(const std::map <std::string, Variant>m) {
    std::ostringstream oss;
    std::string::size_type index = 0;

    oss << "{";
    for (std::map<std::string, Variant>::const_iterator it = m.begin(); it != m.end(); it++) {
        oss << "'" << it->first << "': " << getValue(it->second);
        index++;

        if (index < m.size()) {
            oss << ", ";
        }
    }
    oss << "}";

    return oss.str();
}

std::string formatProperties(const Variant::Map p) {
    std::ostringstream oss;
    std::string::size_type index = 0;

    oss << "{";
    for (std::map<std::string, Variant>::const_iterator it = p.begin(); it != p.end(); it++) {
        oss << "'" << it->first << "': " << getValue(it->second);
        index++;

        if (index < p.size()) {
            oss << ", ";
        }
    }
    oss << "}";

    return oss.str();
}

std::string getValue(const Variant &in_data) {
    std::ostringstream oss;

    VariantType varType = in_data.getType();

    if (varType == ::qpid::types::VAR_STRING) {
        if (in_data.asString().empty()) {
            oss << "None";
        } else {
            oss << "'" << in_data.asString() << "'";
        }
    } else if (varType == ::qpid::types::VAR_BOOL) {
        oss << in_data.asBool();
    } else if (varType == ::qpid::types::VAR_FLOAT) {
        oss << in_data.asFloat();
    } else if (varType == ::qpid::types::VAR_DOUBLE) {
        char buffer [50];
        sprintf(buffer, "%f", in_data.asDouble());
        oss << buffer;
    } else if (varType == ::qpid::types::VAR_UUID) {
        oss << "'" << in_data.asString() << "'";
    } else if (varType == ::qpid::types::VAR_VOID) {
        oss << "None";
    } else if (varType == ::qpid::types::VAR_MAP) {
        oss << formatMap(static_cast<std::map<std::string, Variant> >(in_data.asMap()));
    } else if (varType == ::qpid::types::VAR_LIST) {
        oss << formatList(in_data.asList());
    }
    else {
        // UInt, Int, and default section
        bool flagDone = false;
        std::vector<VariantType> varTypeInts;
        std::vector<VariantType> varTypeUInts;

        varTypeUInts.push_back(::qpid::types::VAR_UINT8);
        varTypeUInts.push_back(::qpid::types::VAR_UINT16);
        varTypeUInts.push_back(::qpid::types::VAR_UINT32);
        varTypeUInts.push_back(::qpid::types::VAR_UINT64);

        varTypeInts.push_back(::qpid::types::VAR_INT8);
        varTypeInts.push_back(::qpid::types::VAR_INT16);
        varTypeInts.push_back(::qpid::types::VAR_INT32);
        varTypeInts.push_back(::qpid::types::VAR_INT64);

        std::vector<VariantType>::iterator varTypeIterator;
        for (varTypeIterator = varTypeUInts.begin(); varTypeIterator != varTypeUInts.end(); varTypeIterator++) {
            if (varType == *varTypeIterator) {
                oss << in_data.asUint64();

                flagDone = true;

                break;
            }
        }
        if (flagDone == false) {
            for (varTypeIterator = varTypeInts.begin(); varTypeIterator != varTypeInts.end(); varTypeIterator++) {
                if (varType == *varTypeIterator) {
                    oss << in_data.asInt64();

                    flagDone = true;

                    break;
                }
            }
        }

        if (flagDone == false) {
            oss << "'" << in_data << "'";
        }
    }

    return oss.str();
}

void printMessageInterop(const Message &message) {
    std::string msgString = "";
    std::ostringstream helper;
    Variant::Map props = message.getProperties();

    try {
        msgString.append("{");


        /** AMQP Header **/
        msgString.append("'durable': ").append(formatBool(message.getDurable()));

        helper << ((uint16_t) message.getPriority());
        msgString.append(", 'priority': ").append(helper.str());
        helper.str("");

        helper << message.getTtl().getMilliseconds();
        msgString.append(", 'ttl': ").append(helper.str()); // TODO ttl/expiration ?
        helper.str("");

        // TODO: Not supported in API, get from properties
        // astitcher: qpid::messaging::Message::getRedlivered() is the boolean neagtion of first-acquirer (although it may not be exact)
        if (props.find("x-amqp-first-acquirer") != props.end()) {
            msgString.append(", 'first-acquirer': ").append(formatBool(props.find("x-amqp-first-acquirer")->second));
        } else {
            msgString.append(", 'first-acquirer': ").append("False");
        }

        // TODO: Not supported in API, get from properties
        if (props.find("x-amqp-delivery-count") != props.end()) {
            msgString.append(", 'delivery-count': ").append(props.find("x-amqp-delivery-count")->second);
        } else {
            msgString.append(", 'delivery-count': ").append("0");
        }


        /** Delivery Annotations **/
        // REMOVED: msgString.append(", 'redelivered': ").append(formatBool(message.getRedelivered()));
        // TODO: Not supported in API, JMS 2.0
        // msgString.append(", 'delivery-time': ").append("0");


        /** AMQP Properties **/
        msgString.append(", 'id': ").append(formatString(message.getMessageId(), true));

        msgString.append(", 'user-id': ").append(formatString(message.getUserId(), true));
      
        // TODO: Not supported in API, get from properties
        if (props.find("x-amqp-to") != props.end()) {
            msgString.append(", 'address': ").append(formatString(props.find("x-amqp-to")->second));
        } else {
            msgString.append(", 'address': ").append("None");
        }

        msgString.append(", 'subject': ").append(formatString(message.getSubject())); // OW: JMS_AMQP_Subject

        msgString.append(", 'reply-to': ").append(formatString(message.getReplyTo().str()));

        msgString.append(", 'correlation-id': ").append(formatString(message.getCorrelationId()));

        msgString.append(", 'content-type': ").append(formatString(message.getContentType()));

        msgString.append(", 'content-encoding': ").append(formatString(message.getContentObject().getEncoding()));

        // TODO: Not supported in API, get from properties
        if (props.find("x-amqp-absolute-expiry-time") != props.end()) {
            msgString.append(", 'absolute-expiry-time': ").append(props.find("x-amqp-absolute-expiry-time")->second);
        } else {
            msgString.append(", 'absolute-expiry-time': ").append("0");
        }

        if (props.find("x-amqp-creation-time") != props.end()) {
            msgString.append(", 'creation-time': ").append(props.find("x-amqp-creation-time")->second);
        } else {
            msgString.append(", 'creation-time': ").append("0");
        }

        // TODO: Not supported in API, get from properties
        if (props.find("x-amqp-group-id") != props.end()) {
            msgString.append(", 'group-id': ").append(formatString(props.find("x-amqp-group-id")->second));
        } else {
            msgString.append(", 'group-id': ").append("None");
        }

        // TODO: Not supported in API, get from properties
        if (props.find("x-amqp-group-sequence") != props.end()) {
            msgString.append(", 'group-sequence': ").append(props.find("x-amqp-group-sequence")->second);
        } else {
            msgString.append(", 'group-sequence': ").append("0");
        }

        // TODO: Not supported in API, get from properties
        if (props.find("x-amqp-reply-to-group-id") != props.end()) {
            msgString.append(", 'reply-to-group-id': ").append(formatString(props.find("x-amqp-reply-to-group-id")->second));
        } else {
            msgString.append(", 'reply-to-group-id': ").append("None");
        }


        /** Application Properties **/
        msgString.append(", 'properties': ").append(formatProperties(props));


        /** Application Data **/
        msgString.append(", 'content': ").append(getValue(message.getContentObject()));

        msgString.append("}");
    } catch (std::exception e) {
        std::cerr << "Error while getting message properties: " << e.what() << std::endl;

        exit(1);
    }

    std::cout << msgString << std::endl;
}

void printStatistics(const Message &message, const Variant::Map &stats) {
#ifdef ENABLE_MODERN
	/* 
	 * NOTE: this is *not* the best way to solve this problem and I will 
	 * implement a more elegant design in the future
	 */
	QpidDecoder decoder = QpidDecoder(message); 
	
	std::ostringstream stream;
	DictWriter writer = DictWriter(&stream);
	
	DictFormatter formatter = DictFormatter();
	formatter.printStatistics(&decoder, &writer, stats);
#else
	Formatter formatter;
	
	Variant::Map tmp = const_cast<Variant::Map &>(stats);
	
	formatter.printStatistics(tmp);
#endif // ENABLE_MODERN
}
	
}}}
