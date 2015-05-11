/*
 * Data.h
 *
 *  Created on: Apr 17, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_DATA_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_DATA_H_

#include <proton/codec.h>

#include <string>
#include <list>
#include <map>

#include "ProtonObject.h"
#include "ProtonError.h"

#include "exceptions/ProtonException.h"
#include "exceptions/DataSerializationException.h"

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

namespace dtests {
namespace proton {
namespace messenger {

using namespace std;

class Data: public ProtonObject {
public:
	Data(size_t capacity);
	Data(pn_data_t *data);
	Data(const string &string);

	virtual ~Data();

	void writeString(const string &data);
	string readString() const;

	void writeMap(const map<string, string> &map);
	map<string, string> readMap() const;

	void writeList(const list<string> &list);
	list<string> readList() const;

	void writeData(const Data &src);

	int size() const;
	bool isEmpty() const;

private:
	typedef ProtonObject super;

	pn_data_t *data;
	bool release;

	void init(size_t capacity);
	void handleError(const string &message);
};

}
}
}

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_DATA_H_ */
