/*
 * BaseException.h
 *
 *  Created on: Apr 22, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_BASEEXCEPTION_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_BASEEXCEPTION_H_

#include <string>

namespace dtests {
namespace common {

using namespace std;

class BaseException {
public:
	BaseException();
	BaseException(const string &message);
	BaseException(const BaseException &other);

	virtual ~BaseException();

	void setMessage(const string &message);
	string getMessage() const;

	BaseException &operator=(const BaseException &rhs);

private:
	string message;

};

}}

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_BASEEXCEPTION_H_ */
