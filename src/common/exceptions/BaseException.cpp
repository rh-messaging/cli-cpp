/*
 * BaseException.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: opiske
 */

#include "BaseException.h"

namespace dtests {
namespace common {

BaseException::BaseException()
    : message()
{

}

BaseException::BaseException(const BaseException& other)
    : message(other.message)
{

}

BaseException::BaseException(const string& message)
    : message(message)
{

}

BaseException::~BaseException()
{

}

void BaseException::setMessage(const string& message)
{
    this->message = message;
}

string BaseException::getMessage() const
{
    return message;
}

BaseException& BaseException::operator=(const BaseException& rhs)
{
    setMessage(rhs.getMessage());

    return *this;
}

}
}
