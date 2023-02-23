/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   STLUtils.h
 * Author: opiske
 *
 * Created on March 16, 2016, 2:13 PM
 */

#ifndef STLUTILS_H
#define STLUTILS_H

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include <vector>
#include <map>
#include <string>
#include <list>

using namespace std;

namespace algo = boost::algorithm;

/**
 * STL utilities
 */

namespace dtests {
namespace common {


/**
 * Splits in the input string and return a vector of its parts
 * @param input the input string to split
 * @param sep the separator characters for the string
 * @return a vector object with each split part
 */
static vector<string> split(const string &input, const string &sep) {
    vector<string> result;
    int curr = 0, next = 0;
    while ((next = input.find_first_of(sep, curr)) != string::npos) {
        result.push_back(input.substr(curr, next - curr));
        curr = next + 1;
    }
    if (curr == 0 || curr < input.size()) {
        result.push_back(input.substr(curr));
    }
    return result;
}


/**
 * Converts an input vector of strings into a map of string based keys and "T" 
 * based values. A clean conversion of string to T is required
 * 
 * @param input the input vector
 * @param output the ouput map
 */
template<typename T>
void convertToMap(const vector<string> &input, map<string, T> &output)
{
    for (size_t i = 0; i < (input.size() - 1); i = i + 2) {
        string key = input[i];
        T value = T(input[i + 1]);

        output[key] = value;
    }
}


/**
 * Converts a key/value properties (ie.: key1:value1, key2:value2, etc) in a 
 * vector object to a to a map
 * @param propertyVector the vector containing the properties
 * @param output the destination map object
 */
template<typename T>
void convertPropertiesToMap(const vector<string> &propertyVector, 
        map<string, T> &output, const string &propertySeparator=":") 
{
    for (size_t i = 0; i < propertyVector.size(); i++) {
        vector<string> tmpVector = split(propertyVector[i], propertySeparator);

        convertToMap(tmpVector, output);
    }
}


} /* namespace common */
} /* namespace dtests */

#endif /* STLUTILS_H */

