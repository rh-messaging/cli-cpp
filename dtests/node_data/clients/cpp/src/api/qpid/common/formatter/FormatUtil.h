/* 
 * File:   FormatUtil.h
 * Author: opiske
 *
 * Created on May 12, 2015, 10:38 AM
 */

#ifndef FORMATUTIL_H
#define	FORMATUTIL_H

#include <qpid/messaging/Message.h>

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

namespace dtests {
namespace qpid {
namespace messaging {
    
void printMessageUpstream(const Message &message, bool verbose = true);
void printMessageDict(const Message &message);
void printStatistics(const Message &message, const Variant::Map &stats);
    
}}}



#endif	/* FORMATUTIL_H */

