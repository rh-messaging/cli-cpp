/* 
 * File:   QpidDecoder.cpp
 * Author: opiske
 * 
 * Created on May 5, 2015, 1:41 PM
 */

#include <qpid/messaging/Address.h>

#include "QpidDecoder.h"

namespace dtests {
namespace qpid {
namespace messaging {


QpidDecoder::QpidDecoder(const Message &message)
    : super(),
    message(message)
{
}

QpidDecoder::QpidDecoder(const QpidDecoder& orig)
    : super(),
      message(orig.message)
{
}

QpidDecoder::~QpidDecoder() {
}

void QpidDecoder::write(Writer *writer,HeaderProperty property, StringReader reader) const {
    string value = (message.*reader)();
    
    
    writer->write(KeyValue(property.name, super::decodeValue(value)));
}


void QpidDecoder::write(Writer *writer,HeaderProperty property, BooleanReader reader) const {
    bool value = (message.*reader)();
    
    writer->write(KeyValue(property.name, super::decodeValue(value)));
}

void QpidDecoder::write(Writer *writer,HeaderProperty property, ShortIntReader reader) const {
    uint8_t value = (message.*reader)();
    
    writer->write(KeyValue(property.name, 
	    super::decodeValue(static_cast<int>(value))));
}


void QpidDecoder::writeReplyTo(Writer *writer) const {
    string replyTo = message.getReplyTo().str();
    
    writer->write(KeyValue(MessageHeader::REPLY_TO.name, 
            super::decodeValue(replyTo)));
}


void QpidDecoder::writeTTL(Writer *writer) const {
    uint64_t ttl = message.getTtl().getMilliseconds();

    std::ostringstream ttl_stream;
    ttl_stream << ttl;
 
    writer->write(MessageHeader::TTL.name);
    writer->write(": ", true);
    writer->write(ttl_stream.str(), true);
    writer->write(", ", true);
}

void QpidDecoder::writeContentSize(Writer *writer) const {
    size_t size = message.getContentSize();
    
    if (size == 0) {
        if (message.getContentType() != ContentType::TEXT_PLAIN) {
            writer->write(MessageHeader::CONTENT_SIZE.name, true);
            writer->write(": ", true);
            writer->write("None", true);
            writer->write(", ", true);
            
            return;
        }
    }
        
    writer->write(KeyValue(MessageHeader::CONTENT_SIZE.name, 
            super::decodeValue(size)));
}

void QpidDecoder::decodeHeader(Writer *writer) const {
    
    
    write(writer, MessageHeader::REDELIVERED, &Message::getRedelivered);
    writeReplyTo(writer);
    write(writer, MessageHeader::SUBJECT, &Message::getSubject);
    write(writer, MessageHeader::CONTENT_TYPE, &Message::getContentType);
    write(writer, MessageHeader::USER_ID, &Message::getUserId);
    write(writer, MessageHeader::ID, &Message::getMessageId);
    write(writer, MessageHeader::CORRELATION_ID, &Message::getCorrelationId);
    
    write(writer, MessageHeader::PRIORITY, &Message::getPriority);
    write(writer, MessageHeader::DURABLE, &Message::getDurable);
    writeTTL(writer);
    writeContentSize(writer);
}

void QpidDecoder::decodeProperties(Writer *writer) const {
    Variant::Map properties = message.getProperties();
    
    writer->startProperties();

    if (properties.size() > 0) {
        decodeValue(writer, properties);
    } else {
        writer->write("{}", true);
    }

    writer->endProperties();
}

void QpidDecoder::decodeContent(Writer *writer) const {
    string content = super::decodeValue(message.getContent());
    writer->write(content);
}


void QpidDecoder::decodeValue(Writer *writer, const Variant &in_data) const {
  VariantType varType = in_data.getType();
  
  if (varType == VAR_STRING) {
    decodeValue(writer, in_data.asString());
  } else if (varType == VAR_BOOL) {
    decodeValue(writer, in_data.asBool());
  } else if (varType == VAR_FLOAT) {
    decodeValue(writer, in_data.asFloat());
  } else if (varType == VAR_DOUBLE) {
    decodeValue(writer, in_data.asDouble());
  } else if (varType == VAR_UUID) {
    decodeValue(writer, in_data.asString());
  } else if (varType == VAR_VOID) {
    // decodeValue(writer, string(""));
    writer->write("None", true);
  } else if (varType == VAR_MAP) {
      super::decodeValue(writer, static_cast<std::map<string, Variant> >(in_data.asMap()));
  } else if (varType == VAR_LIST) {
      decodeValue(writer, in_data.asList());
  }
  else {
    // UInt, Int, and default section
    bool flagDone = false;
    vector<VariantType> varTypeInts;
    vector<VariantType> varTypeUInts;

    varTypeUInts.push_back(VAR_UINT8);
    varTypeUInts.push_back(VAR_UINT16);
    varTypeUInts.push_back(VAR_UINT32);
    varTypeUInts.push_back(VAR_UINT64);

    varTypeInts.push_back(VAR_INT8);
    varTypeInts.push_back(VAR_INT16);
    varTypeInts.push_back(VAR_INT32);
    varTypeInts.push_back(VAR_INT64);

    vector<VariantType>::iterator varTypeIterator;
    for (varTypeIterator = varTypeUInts.begin(); varTypeIterator != varTypeUInts.end(); varTypeIterator++) {
      if (varType == *varTypeIterator) {
          decodeValue(writer, in_data.asUint64());
        flagDone = true;
        break;
      }
    }
    if (flagDone == false) {
      for (varTypeIterator = varTypeInts.begin(); varTypeIterator != varTypeInts.end(); varTypeIterator++) {
        if (varType == *varTypeIterator) {
          decodeValue(writer, in_data.asUint64());
          flagDone = true;
          break;
        }
      }
    }

    if (flagDone == false) {
        writer->writeBogus(in_data);
    }
  }
}

}}}
