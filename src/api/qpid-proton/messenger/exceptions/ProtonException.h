/*
 * ProtonException.h
 *
 *  Created on: Apr 22, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_EXCEPTIONS_PROTONEXCEPTION_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_EXCEPTIONS_PROTONEXCEPTION_H_


#include <string>

#include "BaseException.h"

namespace dtests {
namespace proton {
namespace messenger {

using dtests::common::BaseException;

using namespace std;

class ProtonException : public BaseException {
  public:
    ProtonException(const string &message);
    virtual ~ProtonException();

  private:
    typedef BaseException super;
};

}
}
}

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_EXCEPTIONS_PROTONEXCEPTION_H_ */
