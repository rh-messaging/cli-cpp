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

namespace dtests {
namespace proton {
namespace reactor {

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
void ReactorDecoder::write(Writer *writer, HeaderProperty property, StringReader reader) const {
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
void ReactorDecoder::write(Writer *writer, HeaderProperty property, DataReader reader) const {
    const data &value = (m.*reader)();
    
    writer->write(KeyValue(property.name, this->decodeValue(value)));
}

/**
 * Writes the TTL.
 * @param writer the writer to use
 */
void ReactorDecoder::writeTTL(Writer *writer) const {
    pn_timestamp_t ttl = m.expiry_time().milliseconds;
    
    writer->write(KeyValue(MessageHeader::TTL.name, 
            super::decodeValue(ttl)));
}


/**
 * Writes the content size for plain text messages
 * @param writer the writer to use
 */
void ReactorDecoder::writeContentSize(Writer *writer) const {

    if (m.content_type() != ContentType::TEXT_PLAIN) {
        writer->write(KeyValue(MessageHeader::CONTENT_SIZE.name, "None"));

        return;
    }
    else {
        string body = decodeValue(m.body());
        string len = super::decodeValue(body.length());

        writer->write(KeyValue(MessageHeader::CONTENT_SIZE.name, 
                len));

        return;
    }
}

void ReactorDecoder::decodeHeader(Writer *writer) const {  
    // write(writer, MessageHeader::REDELIVERED, &message::redelivered);
    write(writer, MessageHeader::REPLY_TO, 
            static_cast<StringReader>(&message::reply_to));
    write(writer, MessageHeader::SUBJECT, 
            static_cast<StringReader>(&message::subject));
    write(writer, MessageHeader::CONTENT_TYPE, 
            static_cast<StringReader>(&message::content_type));
    write(writer, MessageHeader::USER_ID, 
            static_cast<StringReader>(&message::user));
    write(writer, MessageHeader::ID, 
            static_cast<DataReader>(&message::id));
    write(writer, MessageHeader::CORRELATION_ID, 
            static_cast<DataReader>(&message::correlation_id));
    
    // Not implemented
    // write(writer, MessageHeader::PRIORITY, &message::priority);
    // write(writer, MessageHeader::DURABLE, &message::durable);
    writeTTL(writer);
    writeContentSize(writer);
}

void ReactorDecoder::decodeProperties(Writer *writer) const {
    // TODO: 
}

string ReactorDecoder::decodeValue(const data &d) const {
    std::ostringstream stream;
    
    stream << d;
    
    return stream.str();
}

void ReactorDecoder::decodeContent(Writer *writer) const {
    string content = decodeValue(m.body());
    writer->write(content);
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */