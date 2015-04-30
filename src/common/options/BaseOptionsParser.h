/*
 * BrokerOptions.h
 *
 *  Created on: Apr 16, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_OPTIONS_BASEOPTIONSPARSER_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_OPTIONS_BASEOPTIONSPARSER_H_

#include <optparse/OptionParser.h>

#include <string>

using namespace std;

using namespace optparse;

class BaseOptionsParser: public OptionParser {
public:
	BaseOptionsParser();
	virtual ~BaseOptionsParser();

	virtual void validate(const Values &options) const = 0;
private:
	typedef OptionParser super;


};

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_OPTIONS_BASEOPTIONSPARSER_H_ */
