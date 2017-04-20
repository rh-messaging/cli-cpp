/* 
 * File:   FormatUtil.h
 * Author: opiske
 *
 * Created on May 12, 2015, 10:38 AM
 */

#ifndef FORMATUTIL_H
#define	FORMATUTIL_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <exception>

#include <qpid/messaging/Message.h>
#include <qpid/messaging/Address.h>
#include <qpid/types/Variant.h>

#include <iostream>

#ifdef ENABLE_MODERN
 #include "formatter/DictFormatter.h"
 #include "formatter/DictWriter.h"

 #include "formatter/UpstreamFormatter.h"
 #include "formatter/UpstreamWriter.h"

 #include "QpidDecoder.h"
#else
 #include "Formatter.h"
#endif // ENABLE_MODERN

using qpid::messaging::Message;
using qpid::types::Variant;
using qpid::types::VariantType;

namespace dtests {
namespace qpid {
namespace messaging {
    
void printMessageUpstream(const Message &message, bool verbose = true);
std::string formatBool(const bool b);
std::string formatBool(const Variant b);
std::string formatString(const std::string s, bool remove_id_prefix = false);
std::string formatMap(const std::map <std::string, Variant>m);
std::string formatProperties(const Variant::Map p);
std::string getValue(const Variant &in_data);
void printMessageDict(const Message &message);
void printMessageInterop(const Message &message);
void printStatistics(const Message &message, const Variant::Map &stats);
    
}}}



#endif	/* FORMATUTIL_H */

