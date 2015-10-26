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

/**
 * BaseOptionsParse offers a common interface for implementing command-line 
 * options parsers for both new and old clients
 */
class BaseOptionsParser : public OptionParser {
  public:
    BaseOptionsParser();
    virtual ~BaseOptionsParser();

    /**
     * Validates the given options 
     * @param options the options to validate
     */
    virtual void validate(const Values &options) const = 0;
  private:
    typedef OptionParser super;


};

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_OPTIONS_BASEOPTIONSPARSER_H_ */
