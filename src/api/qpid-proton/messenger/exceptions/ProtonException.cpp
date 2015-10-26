/*
 * ProtonException.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: opiske
 */

#include "ProtonException.h"

namespace dtests {
namespace proton {
namespace messenger {

using dtests::common::BaseException;

ProtonException::ProtonException(const string &message)
    : super(message)
{

}

ProtonException::~ProtonException()
{

}

}
}
}
