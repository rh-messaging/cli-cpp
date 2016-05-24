/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReactorDecoder.h
 * Author: opiske
 *
 * Created on October 22, 2015, 12:03 PM
 */

#ifndef REACTORDECODER_H
#define REACTORDECODER_H

#include <proton/message.hpp>
#include <proton/types.hpp>
#include <proton/error.hpp>

#include <formatter/AbstractDecoder.h>
#include <ContentType.h>
#include <MessageHeader.h>

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

using proton::message;
using proton::message_id;
using proton::ARRAY;
using proton::MAP;
using proton::LIST;
using proton::DESCRIBED;
using proton::STRING;
using proton::UINT;
using proton::INT;
using proton::DOUBLE;
using proton::value;
using proton::type_id;
using proton::conversion_error;

/*
#if PN_VERSION_MAJOR > 0 && PN_VERSION_MINOR == 12
# define DO_GET(value, str) value.empty() ? value.get(str) : logger(debug) << "Value for property " << property.name << " is empty";
#else
# define DO_GET(value, str) str:string  = value.as_string();
#endif
*/

namespace dtests {
namespace proton {
namespace reactor {

using std::string;
using namespace dtests::common;

/**
 * This class implements a decoder for the reactor API. For additional details
 * please use the documentation from the AbstractDecoder.
 */
class ReactorDecoder : public AbstractDecoder {
  public:
    ReactorDecoder(message &m);
    ReactorDecoder(const ReactorDecoder& orig);
    virtual ~ReactorDecoder();

    virtual void decodeHeader(Writer *writer) const;
    virtual void decodeProperties(Writer *writer) const;
    virtual void decodeContent(Writer *writer) const;
  private:
    typedef AbstractDecoder super;

    typedef string(message::*StringReader)(void) const;
    typedef message_id (message::*MessageIdReader)(void) const;
    typedef uint8_t (message::*Uint8Reader)(void) const;
    typedef uint32_t (message::*Uint32Reader)(void) const;
    typedef bool (message::*BoolReader)(void) const;
    
    message m;
    static Logger logger;

    void write(Writer *writer, HeaderProperty property, Uint8Reader reader) const;
    void write(Writer *writer, HeaderProperty property, StringReader reader) const;
    void write(Writer *writer, HeaderProperty property, MessageIdReader reader) const;
    
    template<typename T, typename V>
    void write(Writer *writer, HeaderProperty property, T reader) const {
        V value = (m.*reader)();

        logger(dtests::common::log::debug) << "Decoding " << property.name << ": ";

        writer->write(KeyValue(property.name, super::decodeValue(value)));
    }
    
    void writeTTL(Writer *writer) const;
    void writeContentSize(Writer *writer) const;

    
    void decodeValue(Writer *writer, value &dec) const;
    
    string decodeValue(const value &dec) const;
    string decodeValue(const string &str) const;

};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* REACTORDECODER_H */

