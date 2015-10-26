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

/**
 * Base exception class
 */
class BaseException {
  public:
    BaseException();

    /**
     * Constructor
     * @param message the exception message
     */
    BaseException(const string &message);

    /**
     * Copy constructor
     * @param other
     */
    BaseException(const BaseException &other);

    virtual ~BaseException();

    /**
     * Sets the exception message
     * @param message the exception message
     */
    void setMessage(const string &message);

    /**
     * Gets the exception message
     * @return the exception message
     */
    string getMessage() const;

    BaseException &operator=(const BaseException &rhs);

  private:
    string message;

};

}
}

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_BASEEXCEPTION_H_ */
