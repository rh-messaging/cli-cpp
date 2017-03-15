/*
 * StringAppendCallback.h
 *
 *  Created on: Mar 8, 2017
 *      Author: rkubis
 */

#ifndef STRING_APPEND_CALLBACK_H
#define STRING_APPEND_CALLBACK_H

#include <optparse/OptionParser.h>

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace dtests {
namespace common {

class StringAppendCallback : public optparse::Callback
{
  public:
    StringAppendCallback();
    virtual ~StringAppendCallback();

    void operator()(const optparse::Option &option, const string &opt, const string &val, const optparse::OptionParser &parser);
    void append(const string &s);
    vector<string> getStrings() const;

    string str;
};

} /* namespace common */
} /* namespace dtests */

#endif /* STRING_APPEND_CALLBACK_H */

