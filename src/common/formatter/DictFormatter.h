/* 
 * File:   DictFormatter.h
 * Author: opiske
 *
 * Created on May 11, 2015, 5:55 PM
 */

#ifndef DTESTS_COMMON_DICTFORMATTER_H
#define DTESTS_COMMON_DICTFORMATTER_H

#include "AbstractFormatter.h"

namespace dtests {
namespace common {

/**
 * Implements a Python dictionary formatter (ie.: saves data in a dict-based 
 * format)
 */
class DictFormatter : public AbstractFormatter {
  public:
    DictFormatter();
    DictFormatter(const DictFormatter& orig);
    virtual ~DictFormatter();

    void printMessage(const AbstractDecoder *decoder, Writer *writer) const;

  private:
    typedef AbstractFormatter super;

};

} /* namespace common */
} /* namespace dtests */

#endif /* DTESTS_COMMON_DICTFORMATTER_H */

