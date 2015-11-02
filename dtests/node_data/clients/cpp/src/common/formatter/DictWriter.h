/* 
 * File:   StringWriter.h
 * Author: opiske
 *
 * Created on May 6, 2015, 4:13 PM
 */

#ifndef DTESTS_COMMON_DICTWRITER_H
#define DTESTS_COMMON_DICTWRITER_H

#include <string>
#include <sstream>

#include "KeyValue.h"
#include "Writer.h"

using std::string;
using std::ostringstream;

/**
 * Python dictionary data writer
 */
class DictWriter : public Writer {
  public:
    DictWriter(ostringstream *stream);
    virtual ~DictWriter();

    // NO-OP method

    void start() {
    };

    // NO-OP method

    void end() {
    };

    void startHeader();
    void endHeader();

    void startProperties();
    void endProperties();

    void startContent();
    void endContent();

    void startMap();
    void endMap();

    void startList();
    void endList();

    void startStatistics();
    void endStatistics();

    void endField();

    void write(const string &str);
    void write(const KeyValue &keyValue);
    void write(const KeyValue &keyValue, bool separator);

    void writeBogus(const string &str);


    void endLine();
    string toString() const;


  private:
    typedef Writer super;

    static const char SECTION_START = '{';
    static const char SECTION_END = '}';

    static const char HEADER_START = SECTION_START;
    static const char HEADER_END = SECTION_END;

    static const char PROPERTIES_START = SECTION_START;
    static const char PROPERTIES_END = SECTION_END;

    static const char CONTENT_START = SECTION_START;
    static const char CONTENT_END = SECTION_END;

    static const char *STATISTICS_START;
    static const char STATISTICS_END = SECTION_END;

    static const char VALUE_START = '\'';
    static const char VALUE_END = '\'';
    static const char VALUE_SEPARATOR = ':';
    static const char FIELD_SEPARATOR = ',';

    ostringstream *stream;
};

#endif /* DTESTS_COMMON_STRINGWRITER_H */

