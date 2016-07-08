/* 
 * File:   AbstractDecoder.h
 * Author: opiske
 *
 * Created on May 5, 2015, 1:22 PM
 */

#ifndef DTESTS_COMMON_ABSTRACTDECODER_H
#define DTESTS_COMMON_ABSTRACTDECODER_H

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "KeyValue.h"
#include "Writer.h"

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

using namespace dtests::common::log;

namespace dtests {
namespace common {

using std::map;
using std::list;
using std::pair;
using std::string;
using std::ostringstream;
using std::scientific;

/**
 * A decoder is a proxy object used to transform messaging objects into a 
 * standard object/pattern that can be used by the formatter
 */
class AbstractDecoder {
  public:
    AbstractDecoder();
    AbstractDecoder(const AbstractDecoder& orig);
    virtual ~AbstractDecoder();

    /**
     * Decodes the message header
     * @param writer a pointer to a writer object that can write the decoded 
     * message header
     */
    virtual void decodeHeader(Writer *writer) const = 0;

    /**
     * Decodes the message properties
     * @param writer a pointer to a writer object that can write the decoded 
     * message properties
     */
    virtual void decodeProperties(Writer *writer) const = 0;

    /**
     * Decodes the message content
     * @param writer a pointer to a writer object that can write the decoded 
     * message content
     */
    virtual void decodeContent(Writer *writer) const = 0;

    /**
     * Decodes the performance statistics
     * @param writer a pointer to a writer object that can write the decoded 
     * message content
     * @param valuesMap the performance statistics map (as in map<string, Y>)
     */
    template <typename T, typename Y>
    void decodeStatistics(Writer *writer, const map<T, Y> &valuesMap) const {
        decodeValue(writer, valuesMap);
    }

  protected:

    /**
     * Decodes a boolean value
     * @param the boolean value
     * @return the decoded value as a string
     */
    virtual string decodeValue(bool) const;

    /**
     * Decodes a boolean value
     * @param writer a pointer to a writer object that can write the decoded 
     * value
     * @param the boolean value
     */
    virtual void decodeValue(Writer *writer, bool) const;

    /**
     * Decodes a string
     * @param the string value
     * @return the decoded string value
     */
    virtual string decodeValue(const string &) const;


    /**
     * Decodes a string
     * @param writer a pointer to a writer object that can write the decoded 
     * value
     * @param the string value
     */
    virtual void decodeValue(Writer *writer, const string &) const;

    /**
     * Decodes a number
     * @param number the number to decode
     * @return the decoded number as a string
     */
    template <typename T>
    string decodeValue(T number) const {
        ostringstream ret;

#ifdef ENABLE_IMPLICIT_CONVERSIONS
        logger(info) << "Decoding number " << number << ".";
#endif
        ret << number;

#ifdef ENABLE_IMPLICIT_CONVERSIONS        
        logger(info) << "Decoded number " << ret.str() << ".";
#endif
        return string(ret.str());
    }

    /**
     * Decodes a number
     * @param number the number to decode
     * @return the decoded number as a string
     */
    virtual string decodeValue(float number) const;

    /**
     * Decodes a number
     * @param writer a pointer to a writer object that can write the decoded 
     * value
     * @param number the number to decode
     */
    virtual void decodeValue(Writer *writer, float number) const;

    /**
     * Decodes a map
     * @param writer a pointer to a writer object that can write the decoded 
     * map
     * @param valuesMap the map to decode
     */
    template <typename T, typename Y>
    void decodeValue(Writer *writer, const map<T, Y> &valuesMap) const {

        if (valuesMap.size() == 0) {
            return;
        }

        writer->startMap();
        for (typename map<T, Y>::const_iterator iterator = valuesMap.begin();
                iterator != valuesMap.end(); iterator++) {
            if (iterator != valuesMap.begin()) {
                writer->endField();
            }

            KeyValue kv = KeyValue(iterator->first,
                    static_cast<T> (iterator->second));

            writer->write(kv, false);

        }
        writer->endMap();
    }

    /**
     * Decodes a list
     * @param writer a pointer to a writer object that can write the decoded 
     * map
     * @param inputList the list to decode
     */
    template <typename T>
    void decodeValue(Writer *writer, const list<T> &inputList) {

        if (inputList.size() == 0) {
            return;
        }

        writer->startList();
        for (typename list<T>::const_iterator iterator = inputList.begin();
                iterator != inputList.end();
                iterator++) {
            decodeValue(writer, static_cast<T> (*iterator));
        }
        writer->endList();
    }

  private:
    static Logger logger;
    
    string quoteStringEscape(const string& a) const;

};

} /* namespace common */
} /* namespace dtests */

#endif /* DTESTS_COMMON_ABSTRACTDECODER_H */

