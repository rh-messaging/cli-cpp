/*
 * StringAppendCallback.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: rkubis
 */

#include "StringAppendCallback.h"

using namespace dtests::common;

StringAppendCallback::StringAppendCallback() :
    str("")
{
}

StringAppendCallback::~StringAppendCallback()
{
    // TODO Auto-generated destructor stub
}
    
void StringAppendCallback::operator()(const optparse::Option &option, const string &opt, const string &val, const optparse::OptionParser &parser)
{
  append(val);
}

void StringAppendCallback::append(const string &s)
{
    if(s.length() > 0) {
        str.append(s);
        str.append(";");
    } else {
        str.append("'';");
    }
}

vector<string> StringAppendCallback::getStrings() const
{
    vector<string> strings;

    string::size_type colon_position = str.find(";");
    string::size_type start = 0;
    string::size_type length;

    if (colon_position != string::npos) {
        while(colon_position != string::npos) {
            colon_position = str.find(";", start);

            length = (colon_position - start);

            if (str.substr(start, length) == "''") {
                strings.push_back("");
            } else if(str.substr(start, length).length() > 0) {
                strings.push_back(str.substr(start, length));
            }
            
            start = (colon_position + 1);
        }
    } else if (str.length() > 0) {
        strings.push_back(str);
    }

    return strings;
}

