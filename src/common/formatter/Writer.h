/* 
 * File:   Writer.h
 * Author: opiske
 *
 * Created on May 5, 2015, 6:26 PM
 */

#ifndef DTESTS_COMMON_WRITER_H
#define DTESTS_COMMON_WRITER_H

#include <string>

#include "KeyValue.h"

namespace dtests {
namespace common {

using std::string;

/**
 * A writer provides a common interface on which data can be written to a stream
 * or medium. It is used by formatters to properly write data in one of the 
 * supported formats.
 */
class Writer {
  public:
    static const char *BOGUS_FORMAT_MESSAGE;

    Writer();
    virtual ~Writer();

    /**
     * Starts writing the data
     */
    virtual void start() = 0;

    /**
     * Ends writing the data
     */
    virtual void end() = 0;
    
    /**
     * Separate data fields
     */
    virtual void separate() = 0;

    /**
     * Starts writing the header section to the medium
     */
    virtual void startHeader() = 0;

    /**
     * Ends writing the header section to the medium
     */
    virtual void endHeader() = 0;

    /**
     * Starts writing the properties section to the medium
     */
    virtual void startProperties() = 0;

    /**
     * Ends writing the properties section to the medium
     */
    virtual void endProperties() = 0;

    /**
     * Starts writing the content section to the medium
     */
    virtual void startContent() = 0;

    /**
     * Ends writing the content section to the medium
     */
    virtual void endContent() = 0;

    /**
     * Starts writing a map data type to the medium
     */
    virtual void startMap() = 0;

    /**
     * Ends writing a map data type to the medium
     */
    virtual void endMap() = 0;


    /**
     * Starts writing a list data type to the medium
     */
    virtual void startList() = 0;

    /**
     * Ends writing a list data type to the medium
     */
    virtual void endList() = 0;

    /**
     * Starts writing statistics information to the medium
     */
    virtual void startStatistics() = 0;

    /**
     * Starts writing statistics information to the medium
     */
    virtual void endStatistics() = 0;

    /**
     * Ends writing field data to the medium
     */
    virtual void endField() = 0;

    /**
     * Write string data to the medium
     * @param str the data to write
     */
    virtual void write(const string &str) = 0;

    /**
     * Write key/value based data to the medium
     * @param keyValue the key/value data to write
     */
    virtual void write(const KeyValue &keyValue) = 0;

    /**
     * Write key/value based data to the medium
     * @param keyValue the key/value data to write
     * @param separator whether to add a separator after writing the data
     */
    virtual void write(const KeyValue &keyValue, bool separator) = 0;


    /**
     * Write bogus-specifc data (eg.: errors, unserializables, etc)
     * @param str the data to write
     */
    virtual void writeBogus(const string &str) = 0;

  private:

};

} /* namespace common */
} /* namespace dtests */

#endif /* WRITER_H */

