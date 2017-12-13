/* 
 * File:   DictFormatter.cpp
 * Author: opiske
 * 
 * Created on May 11, 2015, 5:55 PM
 */

#include "DictFormatter.h"

using namespace dtests::common;

DictFormatter::DictFormatter()
    : super()

{
}

DictFormatter::DictFormatter(const DictFormatter& orig)
    : super()
{
}

DictFormatter::~DictFormatter()
{
}

void DictFormatter::printMessage(const AbstractDecoder *decoder, Writer *writer) const
{
    writer->startHeader();
    decoder->decodeHeader(writer);
    decoder->decodeProperties(writer);

    writer->startContent();
    decoder->decodeContent(writer);
    writer->endContent();
}

std::string DictFormatter::formatBool(bool b) {
    return b ? "True" : "False";
}

std::string DictFormatter::formatBool(scalar b) {
    return get<std::string>(b) == "true" ? "True" : "False";
}

std::string DictFormatter::formatString(std::string s, bool remove_id_prefix) {
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

std::string DictFormatter::formatList(const std::list <scalar>l) {
    std::ostringstream oss;
    std::string::size_type index = 0;

    oss << "[";

    for (std::list<scalar>::const_iterator it = l.begin(); it != l.end(); it++) {
        oss << getValue(*it);
        index++;

        if (index < l.size()) {
            oss << ", ";
        }
    }

    oss << "]";

    return oss.str();
}

std::string DictFormatter::formatMap(const std::map <std::string, scalar>m) {
    std::ostringstream oss;
    std::string::size_type index = 0;

    oss << "{";

    for (std::map<std::string, scalar>::const_iterator it = m.begin(); it != m.end(); it++) {
        oss << "'" << it->first << "': " << getValue(it->second);
        index++;

        if (index < m.size()) {
            oss << ", ";
        }
    }

    oss << "}";

    return oss.str();
}

std::string DictFormatter::formatProperties(message m) {
    std::ostringstream oss;
    std::string::size_type index = 0;

    oss << "{";

    typedef std::map<std::string, proton::scalar> property_map;
    property_map props;
    proton::get(m.properties(), props);

    for (property_map::iterator it = props.begin(); it != props.end(); ++it) {
        oss << "'" << it->first << "': " << getValue(it->second);
        index++;
        if (index < props.size()) {
            oss << ", ";
        }
    }
    oss << "}";

    return oss.str();
}

std::string DictFormatter::getValue(const value &v) {
    type_id type = v.type();
    ostringstream oss;

    switch (type) {
        case ::proton::ARRAY:
        case ::proton::DESCRIBED: {
            break;
        }
        case ::proton::LIST: {
            typedef std::list< ::proton::scalar > property_list;
            property_list value_list;

            ::proton::get(v, value_list);

            oss << formatList(value_list);
            
            break;
        }
        case ::proton::MAP: {
            typedef std::map<std::string, ::proton::scalar> property_map;
            property_map value_map;
            ::proton::get(v, value_map);

            oss << formatMap(value_map);

            break;
        }
        case ::proton::STRING: {
            if (proton::get<string>(v).empty()) {
                oss << "None";
            } else {
                oss << "'" << escapeQuotes(proton::get<string>(v)) << "'";
            }

            break;
        }
        case ::proton::UINT:
        case ::proton::LONG:
        case ::proton::INT:
        case ::proton::DOUBLE: {
            oss << v;

            break;
        }
        default: {
            if (!v.empty()) {
                oss << "'" << v << "'";
            } else {
                oss << "None";
            }
        }        
    }
    
    return oss.str();
}

std::string DictFormatter::escapeQuotes(const std::string s) {
    std::string retVal = s;
    std::string::size_type indexOfQuote = retVal.find("'");

    while(indexOfQuote != std::string::npos) {
        retVal = retVal.replace(indexOfQuote, 1, "\\'");
        indexOfQuote = retVal.find("'", indexOfQuote+2);
    }

    return retVal;
}

void DictFormatter::printMessageInterop(const message &m)
{
    std::string msgString = "";
    std::ostringstream helper;

    try {
        msgString.append("{");


        /** AMQP Header **/
        msgString.append("'durable': ").append(formatBool(m.durable()));

        helper << ((uint16_t) m.priority());
        msgString.append(", 'priority': ").append(helper.str());
        helper.str("");

        helper << m.ttl().milliseconds();
        msgString.append(", 'ttl': ").append(helper.str()); // TODO ttl/expiration ?
        helper.str("");

        msgString.append(", 'first-acquirer': ").append(formatBool(m.first_acquirer()));

        helper << m.delivery_count();
        msgString.append(", 'delivery-count': ").append(helper.str());
        helper.str("");


        /** Delivery Annotations **/
        // REMOVED: msgString.append(", 'redelivered': ").append(formatBool(m.getRedelivered()));
        // TODO: JMS 2.0
        // msgString.append(", 'delivery-time': ").append("0");


        /** AMQP Properties **/
        if (m.id().empty()) {
            msgString.append(", 'id': ").append("None");
        } else {
            msgString.append(", 'id': ").append(formatString(get<std::string>(m.id()), true));
        }

        msgString.append(", 'user-id': ").append(formatString(m.user()));
      
        msgString.append(", 'address': ").append(formatString(m.to()));

        msgString.append(", 'subject': ").append(formatString(m.subject())); // OW: JMS_AMQP_Subject

        msgString.append(", 'reply-to': ").append(formatString(m.reply_to()));

        if (m.correlation_id().empty()) {
            msgString.append(", 'correlation-id': ").append("None");
        } else {
            msgString.append(", 'correlation-id': ").append(formatString(coerce<std::string>(m.correlation_id()), true));
        }

        msgString.append(", 'content-type': ").append(formatString(m.content_type()));

        msgString.append(", 'content-encoding': ").append(formatString(m.content_encoding()));

        helper << m.expiry_time().milliseconds();
        msgString.append(", 'absolute-expiry-time': ").append(helper.str());
        helper.str("");

        helper << m.creation_time().milliseconds();
        msgString.append(", 'creation-time': ").append(helper.str());
        helper.str("");

        msgString.append(", 'group-id': ").append(formatString(m.group_id()));

        helper << m.group_sequence();
        msgString.append(", 'group-sequence': ").append(helper.str());
        helper.str("");

        msgString.append(", 'reply-to-group-id': ").append(formatString(m.reply_to_group_id()));


        /** Application Properties **/
        msgString.append(", 'properties': ").append(formatProperties(m));


        /** Application Data **/
        msgString.append(", 'content': ").append(getValue(m.body()));

        msgString.append("}");
    } catch (std::exception e) {
        std::cerr << "Error while getting message properties: " << e.what() << std::endl;

        // exit(1);
    }

    std::cout << msgString << std::endl;
}
