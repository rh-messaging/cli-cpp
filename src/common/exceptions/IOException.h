/*
 * IOException.h
 *
 *  Created on: Apr 22, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_EXCEPTIONS_IOEXCEPTION_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_EXCEPTIONS_IOEXCEPTION_H_

#include "BaseException.h"

#include <string>

namespace dtests {
namespace common {

using namespace std;

/**
 * Input/ouput exception
 */
class IOException: public BaseException {
public:
	IOException(const string &message);
	virtual ~IOException();

private:
	typedef BaseException super;
};

}}

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_EXCEPTIONS_IOEXCEPTION_H_ */
