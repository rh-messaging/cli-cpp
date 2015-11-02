/* 
 * File:   DictFormatter.h
 * Author: opiske
 *
 * Created on May 11, 2015, 5:55 PM
 */

#ifndef DTESTS_COMMON_UPSTREAMFORMATTER_H
#define DTESTS_COMMON_UPSTREAMFORMATTER_H

#include "AbstractFormatter.h"

namespace dtests {
namespace common {

/**
 * Upstream-specific data formatter
 */
class UpstreamFormatter : public AbstractFormatter {
  public:
    UpstreamFormatter();
    UpstreamFormatter(const UpstreamFormatter& orig);
    virtual ~UpstreamFormatter();

    void printMessage(const AbstractDecoder *decoder, Writer *writer) const;

  private:
    typedef AbstractFormatter super;

};

} /* namespace common */
} /* namespace dtests */

#endif /* DTESTS_COMMON_DICTFORMATTER_H */

