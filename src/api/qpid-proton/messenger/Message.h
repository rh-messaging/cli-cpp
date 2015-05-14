/*
 * Message.h
 *
 *  Created on: Apr 16, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_MESSAGE_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_MESSAGE_H_

#include <proton/messenger.h>

#include <string>
#include <list>
#include <map>

#include "ProtonObject.h"
#include "Data.h"

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

namespace dtests {
namespace proton {
namespace messenger {

using namespace std;

class Messenger;

class Message: public ProtonObject {
friend class Messenger;

public:
	Message(const string &address);
	virtual ~Message();

	void setBody(const Data &data);
	Data getBody() const;

	string getCorrelationId() const;
	void setCorrelationId(const Data &value);

	string getMessageId() const;
	void setMessageId(const Data &value);

	bool isDurable() const;
	void setDurable(bool durable);
	const string getReplyTo() const;
	void setReplyTo(const string& replyTo);
	int getTimeToLive() const;
	void setTimeToLive(int timeToLive);
	const string getUserId() const;
	void setUserId(const string& userId);

	void setPriority(short priority);
	int getPriority() const;

	void setSubject(const string &subject);
	string getSubject() const;

	void setContentType(const string &contentType);
	string getContentType() const;

	void setProperties(const map<string, string> &properties);
	map<string, string> getProperties() const;

private:
	typedef ProtonObject super;
	typedef pn_atom_t (*GetIdFunc)(pn_message_t *);
	typedef pn_data_t *(*GetDataFunc)(pn_message_t *);

	pn_message_t *message;

	void init(const string &address);

	string getId(GetDataFunc getDataFunc, GetIdFunc getIdFunc, const string &idTypeName) const;
	void setId(GetDataFunc getDataFunc, const string& dataName, const Data &value);

	pn_message_t *getMessage() const ;


};

}}}
#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_MESSAGE_H_ */
