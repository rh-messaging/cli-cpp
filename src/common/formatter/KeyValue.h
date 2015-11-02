/* 
 * File:   KeyValue.h
 * Author: opiske
 *
 * Created on May 5, 2015, 6:31 PM
 */

#ifndef DTESTS_COMMON_KEYVALUE_H
#define DTESTS_COMMON_KEYVALUE_H

#include <string>
#include <map>

namespace dtests {
namespace common {

using std::pair;
using std::string;

/**
 * An abstraction for key/value data
 */
typedef pair<string, string> KeyValue;

} /* namespace common */
} /* namespace dtests */

#endif /* DTESTS_COMMON_KEYVALUE_H */

