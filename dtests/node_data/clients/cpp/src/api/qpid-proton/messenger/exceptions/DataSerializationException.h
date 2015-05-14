/*
 * DataSerializationException.h
 *
 *  Created on: Apr 22, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_EXCEPTIONS_DATASERIALIZATIONEXCEPTION_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_EXCEPTIONS_DATASERIALIZATIONEXCEPTION_H_


#include <string>

#include "ProtonException.h"

namespace dtests {
namespace proton {
namespace messenger {

using std::string;

class DataSerializationException: public ProtonException {
public:
	DataSerializationException(const string &message);
	virtual ~DataSerializationException();

private:
	typedef ProtonException super;

};

}}}

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_EXCEPTIONS_DATASERIALIZATIONEXCEPTION_H_ */
