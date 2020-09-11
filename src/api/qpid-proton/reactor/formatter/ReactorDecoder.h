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

#include <formatter/AbstractDecoder.h>
#include <ContentType.h>
#include <MessageHeader.h>

using proton::message;
using proton::message_id;
using proton::amqp_string;
using proton::data;
using proton::MAP;

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
    typedef const data &(message::*DataReader)(void) const;
    typedef message_id (message::*MessageIdReader)(void) const;

    void write(Writer *writer, HeaderProperty property, StringReader reader) const;
    void write(Writer *writer, HeaderProperty property, DataReader reader) const;
    void write(Writer *writer, HeaderProperty property, MessageIdReader reader) const;
    
    void writeTTL(Writer *writer) const;
    void writeContentSize(Writer *writer) const;

    message m;

    string decodeValue(const data &data) const;
    string decodeValue(const amqp_string &str) const;

};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* REACTORDECODER_H */

