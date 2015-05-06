/* 
 * File:   MessageHeader.h
 * Author: opiske
 *
 * Created on May 5, 2015, 4:16 PM
 */

#ifndef MESSAGEHEADER_H
#define	MESSAGEHEADER_H

typedef struct header_property_t_ {
    const char *name;
} HeaderProperty;

/**
 * AMQP message headers
 */
class MessageHeader {
public:
    static HeaderProperty REDELIVERED;
    static HeaderProperty REPLY_TO;
    static HeaderProperty SUBJECT;
    static HeaderProperty CONTENT_TYPE;
    static HeaderProperty ID;
    static HeaderProperty CORRELATION_ID;
    static HeaderProperty PRIORITY;
    static HeaderProperty DURABLE;
    static HeaderProperty TTL;
    static HeaderProperty USER_ID;
    static HeaderProperty CONTENT_SIZE;
    
    
    // Unused atm
    static HeaderProperty DELIVERY_MODE;  
    static HeaderProperty EXPIRATION;
    static HeaderProperty BODY_LENGTH;
    static HeaderProperty GROUP_ID;
    static HeaderProperty GROUP_SEQ;
    static HeaderProperty BROKER_PATH;
    static HeaderProperty ORIGINAL_DESTINATIION;
    
    static HeaderProperty printableProperties[];
    
private:
    
    MessageHeader() {} 
    MessageHeader(const MessageHeader& orig) {}
    virtual ~MessageHeader() {}

};

#endif	/* MESSAGEHEADER_H */

