/*
 * DataSerializationException.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: opiske
 */

#include "DataSerializationException.h"

namespace dtests {
namespace proton {
namespace messenger {

DataSerializationException::DataSerializationException(const string &message)
	: super(message)
{


}

DataSerializationException::~DataSerializationException() {

}

}}}
