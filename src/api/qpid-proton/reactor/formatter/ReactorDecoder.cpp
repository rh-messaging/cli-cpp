/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReactorDecoder.cpp
 * Author: opiske
 * 
 * Created on October 22, 2015, 12:03 PM
 */

#include "ReactorDecoder.h"

using namespace dtests::proton::reactor;
using namespace dtests::common;
using namespace dtests::common::log;

Logger ReactorDecoder::logger = LoggerWrapper::getLogger();

ReactorDecoder::ReactorDecoder(message &msg)
    : super(),
    m(msg)
{
}

ReactorDecoder::ReactorDecoder(const ReactorDecoder& orig)
{
}

ReactorDecoder::~ReactorDecoder()
{
}

/**
 * Writes a header property.
 * @param writer the writer to use
 * @param property the header property to write
 * @param reader The reader to use. The reader is a pointer to a member function 
 * that can access the header property data (ie.: the class getter).
 */
void ReactorDecoder::write(Writer *writer, HeaderProperty property, StringReader reader) const
{
    string value = (m.*reader)();

    writer->write(KeyValue(property.name, super::decodeValue(value)));
}



/**
 * Writes a header property.
 * @param writer the writer to use
 * @param property the header property to write
 * @param reader The reader to use. The reader is a pointer to a member function 
 * that can access the header property data (ie.: the class getter).
 */
void ReactorDecoder::write(Writer *writer, HeaderProperty property, MessageIdReader reader) const
{
    const message_id value = (m.*reader)();

    logger(debug) << "Decoding " << property.name << ": ";

    writer->write(KeyValue(property.name, super::decodeValue(value.as_uint())));
}

/**
 * Writes the TTL.
 * @param writer the writer to use
 */
void ReactorDecoder::writeTTL(Writer *writer) const
{
    pn_timestamp_t ttl = m.expiry_time().milliseconds();
    
    writer->write(KeyValue(MessageHeader::TTL.name,
            super::decodeValue(ttl)));
}

/**
 * Writes the content size for plain text messages
 * @param writer the writer to use
 */
void ReactorDecoder::writeContentSize(Writer *writer) const
{

    if (m.content_type() != ContentType::TEXT_PLAIN) {
        writer->write(KeyValue(MessageHeader::CONTENT_SIZE.name, "None"));

        return;
    } else {
        // string body = decodeValue(writer, m.body());
        string body = "";
        string len = super::decodeValue(body.size());

        writer->write(KeyValue(MessageHeader::CONTENT_SIZE.name,
                len));

        return;
    }
}

void ReactorDecoder::decodeHeader(Writer *writer) const
{
    write<Uint32Reader, uint32_t>(writer, MessageHeader::REDELIVERED, 
            static_cast<Uint32Reader> (&message::delivery_count));
    write(writer, MessageHeader::REPLY_TO,
            static_cast<StringReader> (&message::reply_to));
    write(writer, MessageHeader::SUBJECT,
            static_cast<StringReader> (&message::subject));
    write(writer, MessageHeader::CONTENT_TYPE,
            static_cast<StringReader> (&message::content_type));
    write(writer, MessageHeader::USER_ID,
            static_cast<StringReader> (&message::user_id));
    write(writer, MessageHeader::ID,
            static_cast<MessageIdReader> (&message::id));
    write(writer, MessageHeader::CORRELATION_ID,
            static_cast<MessageIdReader> (&message::correlation_id));

    write<Uint8Reader, uint8_t>(writer, MessageHeader::PRIORITY, 
            static_cast<Uint8Reader> (&message::priority));
    write<BoolReader, bool>(writer, MessageHeader::DURABLE, 
            static_cast<BoolReader> (&message::durable));
    writeTTL(writer);
    writeContentSize(writer);
}

void ReactorDecoder::decodeProperties(Writer *writer) const
{
    logger(debug) << "Decoding message properties";
    writer->startProperties();
    // The call below should be replaced with one that can handle regular maps
    // decodeValue(writer, m.application_properties().);
    writer->endProperties();
}

void ReactorDecoder::decodeValue(Writer *writer, value &value) const
{
    type_id type = value.type();
        
    switch (type) {
        case ARRAY:
        case DESCRIBED: 
            logger(debug) << "(o) Type id: " << type;
            break;

        case LIST: {
            logger(debug) << "(m) Type id: " << type;
        
            writer->startList();
            logger(debug) << "(m) Type id: " << type;
            // dec >> s;
            
            /*
            logger(debug) << "(m) Size: " << s.size;
            
            value.get();
            for (size_t i = 0; i < s.size; i++) {
                decodeValue(writer, dec);
                writer->write(": ");
                
                if (i < s.size) {
                    writer->endField();
                }
            }
            */
            
            // dec >> finish();
            writer->endMap();        
        }
        case MAP:
        {
            
            writer->startMap();
            logger(debug) << "(m) Type id: " << type;
            /*
            dec >> s;
            
            logger(debug) << "(m) Size: " << s.size;
            for (size_t i = 0; i < s.size/2; i++) {
                decodeValue(writer, dec);
                writer->separate();
                decodeValue(writer, dec);
                
                if (i < ((s.size/2) - 1)) {
                    writer->endField();
                }
            }
            
            dec >> finish();
            */
            writer->endMap();
        }
        default:
        {
            /*
            std::ostringstream stream;
            
            value v;
            dec >> v;
            stream << v;

            writer->write(stream.str());            
             */
        }
    }
}

string ReactorDecoder::decodeValue(const value &value) const {
    type_id type = value.type();
    ostringstream s;

    switch (type) {
        case ARRAY:
        case DESCRIBED:
            logger(debug) << "(o) Type id: " << type;
            break;

        case LIST: {
            logger(debug) << "(m) Type id: " << type;

            std::list<string> l = std::list<string>();

            value.get_sequence(l);
            logger(debug) << "(m) Type id: " << type;

            logger(debug) << "(m) Size: " << l.size();

            for (size_t i = 0; i < l.size(); i++) {
                // decodeValue(writer, dec);
                // writer->write(": ");

                // if (i < s.size) {
                //    writer->endField();
                // }
            }

            // dec >> finish();
            // writer->endMap();
        }
        case MAP:
        {
            // writer->startMap();
            logger(debug) << "(m) Type id: " << type;
            /*
            dec >> s;

            logger(debug) << "(m) Size: " << s.size;
            for (size_t i = 0; i < s.size/2; i++) {
                decodeValue(writer, dec);
                writer->separate();
                decodeValue(writer, dec);

                if (i < ((s.size/2) - 1)) {
                    writer->endField();
                }
            }

            dec >> finish();
            */
            // writer->endMap();
        }
        case STRING: {
            s << value.as_string();
            logger(debug) << "(m) String: ";
            break;
        }
        case UINT: {
            s << value.as_uint();
            logger(debug) << "(m) UInt: ";
            break;
        }
        case INT: {
            s << value.as_int();
            logger(debug) << "(m) UInt: ";
            break;
        }
        case DOUBLE: {
            s << value.as_double();
            logger(debug) << "(m) UInt: ";
            break;
        }
        default: {
            s << value.as_string();
            logger(debug) << "(m) Other: ";
        }
    }

    return s.str();
}


// TODO: this is likely to be deprecated and should be removed
string ReactorDecoder::decodeValue(const string &str) const
{
    return str;
}

void ReactorDecoder::decodeContent(Writer *writer) const
{
    if (m.body().type() == MAP) {
        logger(debug) << "Decoding a map message";
    }

    string content = decodeValue(m.body());

    writer->write(content);
}