/*
 * ProtonObject.h
 *
 *  Created on: Apr 28, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_PROTONOBJECT_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_PROTONOBJECT_H_

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

namespace dtests {
namespace proton {
namespace messenger {

class ProtonObject {
public:
	ProtonObject();
    virtual ~ProtonObject();
protected:
    static Logger logger;
};

}}}

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_PROTONOBJECT_H_ */
