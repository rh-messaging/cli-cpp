/*
 * IOException.h
 *
 *  Created on: Apr 22, 2015
 *      Author: opiske
 */

#ifndef DTESTS_COMMON_IOEXCEPTION_H_
#define DTESTS_COMMON_IOEXCEPTION_H_

#include "BaseException.h"

#include <string>

namespace dtests {
namespace common {

using namespace std;

/**
 * Input/ouput exception
 */
class IOException : public BaseException {
  public:
    IOException(const string &message);
    virtual ~IOException();

  private:
    typedef BaseException super;
};

}
}

#endif /* DTESTS_COMMON_IOEXCEPTION_H_ */
