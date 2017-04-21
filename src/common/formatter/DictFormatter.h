/* 
 * File:   DictFormatter.h
 * Author: opiske
 *
 * Created on May 11, 2015, 5:55 PM
 */

#ifndef DTESTS_COMMON_DICTFORMATTER_H
#define DTESTS_COMMON_DICTFORMATTER_H

#include <proton/message.hpp>
#include <proton/message_id.hpp>
#include <proton/scalar.hpp>
#include <proton/value.hpp>

#include "AbstractFormatter.h"

using proton::message;
using proton::message_id;
using proton::scalar;
using proton::get;
using proton::value;
using proton::type_id;

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
    std::string formatBool(bool b);
    std::string formatBool(scalar b);
    std::string formatString(std::string s, bool remove_id_prefix = false);
    std::string formatList(const std::list <value>l);
    std::string formatMap(const std::map <std::string, value>m);
    std::string formatProperties(message::property_map p);
    std::string getValue(const value &v);
    void printMessageInterop(const message &m);

  private:
    typedef AbstractFormatter super;

};

} /* namespace common */
} /* namespace dtests */

#endif /* DTESTS_COMMON_DICTFORMATTER_H */

