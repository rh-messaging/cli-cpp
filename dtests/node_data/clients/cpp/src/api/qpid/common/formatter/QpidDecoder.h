/* 
 * File:   QpidDecoder.h
 * Author: opiske
 *
 * Created on May 5, 2015, 1:41 PM
 */

#ifndef QPIDDECODER_H
#define	QPIDDECODER_H

#include <vector>

#include <qpid/messaging/Message.h>
#include <qpid/types/Variant.h>

#include <formatter/AbstractDecoder.h>
#include <ContentType.h>
#include <MessageHeader.h>

using std::vector;
using qpid::messaging::Message;

using namespace qpid::types;



namespace dtests {
namespace qpid {
namespace messaging {

using namespace dtests::common;
    
class QpidDecoder: public AbstractDecoder {
public:
    QpidDecoder(const Message &message);
    QpidDecoder(const QpidDecoder& orig);
    virtual ~QpidDecoder();
   
    virtual void decodeHeader(Writer *writer) const;
    virtual void decodeProperties(Writer *writer) const;
    virtual void decodeContent(Writer *writer) const;
    
private:
    typedef AbstractDecoder super;
    typedef const string &(Message::*StringReader)(void) const;
    typedef bool (Message::*BooleanReader)(void) const;
    typedef uint8_t (Message::*ShortIntReader)(void) const;
            
    Message message;
    
    void writeReplyTo(Writer *writer) const;
    void writeTTL(Writer *writer) const;
    void writeContentSize(Writer *writer) const;
    
    void write(Writer *writer, HeaderProperty property, StringReader reader) const;
    void write(Writer *writer, HeaderProperty property, BooleanReader reader) const;
    void write(Writer *writer, HeaderProperty property, ShortIntReader reader) const;
    
    virtual void decodeValue(Writer *writer, const Variant &variant) const;
};

} } }

#endif	/* QPIDDECODER_H */

