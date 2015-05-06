/* 
 * File:   AbstractDecoder.cpp
 * Author: opiske
 * 
 * Created on May 5, 2015, 1:22 PM
 */

#include "AbstractDecoder.h"

AbstractDecoder::AbstractDecoder() {
}

AbstractDecoder::AbstractDecoder(const AbstractDecoder& orig) {
}

AbstractDecoder::~AbstractDecoder() {
}

string AbstractDecoder::decodeValue(bool in_data) const {
  if (in_data == true) {
    return string("True");
  }
  
  return string("False");
}


void AbstractDecoder::decodeValue(Writer *writer, bool in_data) const {
  writer->write(decodeValue(in_data));
}

string AbstractDecoder::quoteStringEscape(const string &a) const {
  char pattern = '\'';
  char range_low = 32;
  char range_high = 126;
  string int_result = "";
  
  for (string::const_iterator it = a.begin(); it != a.end(); it++) {
    if (*it < range_low || *it > range_high) {
      ostringstream tmp;
      
      /*
        \\\\ -- because of python eval escaping
       */
      
      tmp << "\\\\x" << std::setfill('0') << std::hex << std::setw(2) << int(*it);
      int_result.append(tmp.str());
    } else {
      if (*it == pattern) {
        int_result.push_back('\\');
      }
      int_result.push_back(*it);
    }
  }

  return int_result;
}

string AbstractDecoder::decodeValue(const string &in_data) const {
  if (in_data.empty()) {
    return string("None");
  }
  
  // string int_result = string("'");
  
  // int_result.append(quoteStringEscape(in_data));
  // int_result.append("'");

  return in_data;
}

void AbstractDecoder::decodeValue(Writer *writer, const string &in_data) const {
  writer->write(decodeValue(in_data));
}

string AbstractDecoder::decodeValue(float number) const {
    ostringstream ret;

    ret << scientific << number;

    return string(ret.str());
}


void AbstractDecoder::decodeValue(Writer *writer, float number) const {
    writer->write(decodeValue(number));
}