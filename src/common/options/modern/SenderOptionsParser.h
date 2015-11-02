/*
 * ControlOptions.h
 *
 *  Created on: Apr 16, 2015
 *      Author: opiske
 */

#ifndef DTESTS_COMMON_SENDEROPTIONSPARSER_H_
#define DTESTS_COMMON_SENDEROPTIONSPARSER_H_

#include <optparse/OptionParser.h>

#include "options/modern/ModernOptionsParser.h"

namespace dtests {
namespace common {

/**
 * SenderOptionsParser offers a common interface for implementing command-line 
 * options parsers for both newer sender C++ clients
 */
class SenderOptionsParser : public ModernOptionsParser {
  public:
    SenderOptionsParser();


    virtual ~SenderOptionsParser();

    virtual void validate(const Values &options) const;

  private:
    typedef ModernOptionsParser super;
};

} /* namespace common */
} /* namespace dtests */

#endif /* DTESTS_COMMON_SENDEROPTIONSPARSER_H_ */
