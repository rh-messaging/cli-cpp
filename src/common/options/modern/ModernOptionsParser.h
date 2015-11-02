/*
 * ModernOptionsParser.h
 *
 *  Created on: Apr 20, 2015
 *      Author: opiske
 */

#ifndef DTESTS_COMMON_MODERNOPTIONSPARSER_H_
#define DTESTS_COMMON_MODERNOPTIONSPARSER_H_

#include <optparse/OptionParser.h>

#include <string>

#include "../BaseOptionsParser.h"

namespace dtests {
namespace common {

using namespace optparse;

/**
 * ModernOptionsParser offers a common interface for implementing command-line 
 * options parsers for both newer C++ clients
 */
class ModernOptionsParser : public BaseOptionsParser {
  public:
    ModernOptionsParser();
    virtual ~ModernOptionsParser();

    virtual void validate(const Values &options) const;
  private:
    typedef BaseOptionsParser super;

};

} /* namespace common */
} /* namespace dtests */

#endif /* DTESTS_COMMON_MODERNOPTIONSPARSER_H_ */
