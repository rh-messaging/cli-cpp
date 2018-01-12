/*
 * ReceiverOptionsParser.h
 *
 *  Created on: Apr 16, 2015
 *      Author: opiske
 */

#ifndef DTESTS_COMMON_RECEIVEROPTIONSPARSER_H_
#define DTESTS_COMMON_RECEIVEROPTIONSPARSER_H_

#include <optparse/OptionParser.h>

#include <string>
#include <algorithm>

#include "options/modern/ModernOptionsParser.h"

namespace dtests {
namespace common {

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

} /* namespace common */
} /* namespace dtests */

#endif /* DTESTS_COMMON_RECEIVEROPTIONSPARSER_H_ */
