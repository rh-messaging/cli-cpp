/*
 * ModernOptionsParser.h
 *
 *  Created on: Apr 20, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_OPTIONS_MODERN_MODERNOPTIONSPARSER_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_OPTIONS_MODERN_MODERNOPTIONSPARSER_H_

#include <optparse/OptionParser.h>

#include <string>

#include "../BaseOptionsParser.h"

using namespace std;

using namespace optparse;

class ModernOptionsParser: public BaseOptionsParser {
public:
	ModernOptionsParser();
	virtual ~ModernOptionsParser();

	virtual void validate(const Values &options) const;
private:
	typedef BaseOptionsParser super;

};

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_OPTIONS_MODERN_MODERNOPTIONSPARSER_H_ */
