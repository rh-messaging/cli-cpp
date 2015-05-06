/* 
 * File:   MessageHeader.cpp
 * Author: opiske
 * 
 * Created on May 5, 2015, 4:16 PM
 */

#include "MessageHeader.h"

HeaderProperty MessageHeader::REDELIVERED = { "redelivered" };
HeaderProperty MessageHeader::REPLY_TO = { "reply_to" };

HeaderProperty MessageHeader::MessageHeader::SUBJECT = { "subject" };
HeaderProperty MessageHeader::CONTENT_TYPE  = { "content_type" };
HeaderProperty MessageHeader::ID  = { "id" };
HeaderProperty MessageHeader::CORRELATION_ID  = { "correlation_id" };
HeaderProperty MessageHeader::PRIORITY  = { "priority" };
HeaderProperty MessageHeader::DURABLE  = { "durable" };
HeaderProperty MessageHeader::TTL  = { "ttl" };
HeaderProperty MessageHeader::USER_ID  = { "user_id" };
HeaderProperty MessageHeader::CONTENT_SIZE  = { "size" };

HeaderProperty MessageHeader::DELIVERY_MODE = { "delivery_mode" };
HeaderProperty MessageHeader::EXPIRATION = { "expiration" };
HeaderProperty MessageHeader::BODY_LENGTH = { "body_length" };
HeaderProperty MessageHeader::GROUP_ID = { "group_id" };
HeaderProperty MessageHeader::GROUP_SEQ = { "group_seq" };
HeaderProperty MessageHeader::BROKER_PATH = { "broker_path" };
HeaderProperty MessageHeader::ORIGINAL_DESTINATIION = { "original_destination" };


HeaderProperty MessageHeader::printableProperties[] = {
        REDELIVERED,
        REPLY_TO,
        SUBJECT,
        CONTENT_TYPE,
        ID,
        CORRELATION_ID,
        PRIORITY,
        DURABLE,
        TTL,
        CONTENT_SIZE,
};