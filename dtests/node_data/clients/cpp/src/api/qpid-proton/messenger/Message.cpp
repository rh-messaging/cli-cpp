/*
 * Message.cpp
 *
 *  Created on: Apr 16, 2015
 *      Author: opiske
 */

#include "Message.h"

namespace dtests {
namespace proton {
namespace messenger {

using namespace dtests::common::log;

Message::Message(const string &address)
    : super(),
    message(NULL)
{
    init(address);
}

Message::~Message()
{
    pn_message_free(message);
}

void Message::init(const string &address)
{
    logger(trace) << "Creating a new message object for " << address;
    message = pn_message();

    if (message == NULL) {
        throw ProtonException("Unable to create a new message object");
    }

    pn_message_set_address(message, address.c_str());
}

void Message::setBody(const Data &data)
{
    pn_data_t *body = pn_message_body(message);

    Data d = Data(body);
    d.writeData(data);
}

Data Message::getBody() const
{
    pn_data_t *body = pn_message_body(message);

    /*
            char buffer[1024];
            size_t buffsize = sizeof(buffer);
            const char* subject = pn_message_get_subject(message);
        pn_data_format(body, buffer, &buffsize);

        logger(debug) << "Address: " << pn_message_get_address(message);
        logger(debug) << "Subject: " <<
                    subject ? subject : "(no subject)";
        logger(debug) << "Content: " << buffer;
     */

    return Data(body);
}

string Message::getId(GetDataFunc getDataFunc, GetIdFunc getIdFunc,
        const string &idTypeName) const
{
    pn_data_t *id = getDataFunc(message);

    Data d = Data(id);

    if (d.isEmpty()) {
        logger(warning) << "The " << idTypeName << " ID is not set";

        return string("");
    }

    pn_atom_t data = getIdFunc(message);
    string ret = "";

    switch (data.type) {
    case PN_STRING:
    {
        logger(debug) << "Processing string-based " <<
                idTypeName << " ID";

        logger(trace) << "Obtained " << idTypeName << " ID: "
                << d.readString();

        ret = d.readString();
        break;
    }
    default:
    {
        logger(warning) << "Unhandled data type " << data.type;
    }
    }

    return ret;
}

void Message::setId(GetDataFunc getDataFunc, const string& dataName,
        const Data &value)
{
    pn_data_t *id = getDataFunc(message);

    Data d = Data(id);

    if (!d.isEmpty()) {
        logger(warning) << "The " << dataName << " ID seems to" <<
                "already set. It's data structure size is " << d.size() <<
                " instead of 0";

        logger(debug) << "Previously set correlation ID data: " <<
                d.readString();
    }

    d.writeData(value);
}

string Message::getCorrelationId() const
{
    return getId(pn_message_correlation_id, pn_message_get_correlation_id,
            "correlation");
}

void Message::setCorrelationId(const Data &value)
{
    setId(pn_message_correlation_id, "correlation", value);
}

string Message::getMessageId() const
{
    return getId(pn_message_id, pn_message_get_id,
            "message");
}

void Message::setMessageId(const Data& value)
{
    setId(pn_message_id, "message", value);
}

bool Message::isDurable() const
{
    return pn_message_is_durable(message);
}

void Message::setDurable(bool durable)
{
    pn_message_set_durable(message, durable);

}

const string Message::getReplyTo() const
{
    return string(pn_message_get_reply_to(message));
}

void Message::setReplyTo(const string& replyTo)
{
    pn_message_set_reply_to(message, replyTo.c_str());
}

int Message::getTimeToLive() const
{
    return pn_message_get_ttl(message);
}

// TODO: check this cast ... function call uses a pn_millis_t

void Message::setTimeToLive(int timeToLive)
{
    pn_message_set_ttl(message, timeToLive);
}

const string Message::getUserId() const
{
    pn_bytes_t bytes = pn_message_get_user_id(message);

    return string(bytes.start);
}

void Message::setUserId(const string& userId)
{
    pn_bytes_t bytes = pn_bytes(userId.length(), userId.c_str());

    pn_message_set_user_id(message, bytes);
}

void Message::setPriority(short priority)
{
    pn_message_set_priority(message, priority);
}

int Message::getPriority() const
{
    return pn_message_get_priority(message);
}

void Message::setSubject(const string& subject)
{
    pn_message_set_subject(message, subject.c_str());
}

string Message::getSubject() const
{
    const char *subject = pn_message_get_subject(message);
    if (subject == NULL) {
        return "";
    }

    return string(subject);
}

void Message::setContentType(const string& contentType)
{
    pn_message_set_content_type(message, contentType.c_str());
}

string Message::getContentType() const
{
    return pn_message_get_content_type(message);
}

void Message::setProperties(const map<string, string> &properties)
{
    pn_data_t *propertiesData = pn_message_properties(message);

    if (propertiesData == NULL) {
        logger(trace) << "The message does not seem to have any "
                << "property set";
    }

    Data data = Data(propertiesData);

    data.writeMap(properties);
}

map<string, string> Message::getProperties() const
{
    return map<string, string>();
}

pn_message_t* Message::getMessage() const
{
    return message;
}

}
}
}
