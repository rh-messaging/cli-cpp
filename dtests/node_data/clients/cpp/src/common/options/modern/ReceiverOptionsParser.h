/*
 * ControlOptions.h
 *
 *  Created on: Apr 16, 2015
 *      Author: opiske
 */

#ifndef DTESTS_COMMON_RECEIVEROPTIONSPARSER_H_
#define DTESTS_COMMON_RECEIVEROPTIONSPARSER_H_

#include <optparse/OptionParser.h>

#include "options/modern/ModernOptionsParser.h"

/**
 * ReceiverOptionsParser offers a common interface for implementing command-line 
 * options parsers for both newer receiver C++ clients
 */
class ReceiverOptionsParser : public ModernOptionsParser {
  public:
    ReceiverOptionsParser();


    virtual ~ReceiverOptionsParser();

    virtual void validate(const Values &options) const;

  private:
    typedef ModernOptionsParser super;
};

#endif /* DTESTS_COMMON_RECEIVEROPTIONSPARSER_H_ */
