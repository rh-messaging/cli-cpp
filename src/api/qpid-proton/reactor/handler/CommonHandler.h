/*
 * SimpleMessagingHandler.h
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#ifndef COMMON_HANDLER_H_
#define COMMON_HANDLER_H_

#include <proton/container.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/connection.hpp>

#include <proton/url.hpp>

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

using proton::messaging_handler;
using proton::url;

namespace dtests {
namespace proton {
namespace reactor {

using std::string;


class CommonHandler: public messaging_handler {
public:
	CommonHandler(const string &url);
	virtual ~CommonHandler();

protected:
	static Logger logger;
	url broker_url;
	
private:
	typedef messaging_handler super;

        
	
};



} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* COMMON_HANDLER_H_ */
