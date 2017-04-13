/* 
 * File:   StringWriter.h
 * Author: opiske
 *
 * Created on May 6, 2015, 4:13 PM
 */

#ifndef DTESTS_COMMON_UPSTREAMWRITER_H
#define DTESTS_COMMON_UPSTREAMWRITER_H

#include <string>
#include <sstream>

#include "KeyValue.h"
#include "Writer.h"

namespace dtests {
namespace common {

using std::string;
using std::ostringstream;

/**
 * Upstream-specific data writer
 */
class UpstreamWriter : public Writer {
  public:
    UpstreamWriter(ostringstream *stream);
    virtual ~UpstreamWriter();

    void start();
    void end();

    // NO-OP

    void startHeader() {
    }

    // NO-OP

    void endHeader() {
    }
    
    // NO-OP

    void separate() {
    }

    void startProperties();

    // NO-OP

    void endProperties() {
    };

    void startContent();

    void endContent() {
    };

    void startMap();
    void endMap();

    void startList();
    void endList();

    // NO-OP

    void startStatistics() {
    };

    // NO-OP

    void endStatistics() {
    };

    void endField();

    void write(const string &str, const bool raw = false);
    void write(const KeyValue &keyValue);
    void write(const KeyValue &keyValue, bool separator);

    void writeBogus(const string &str);


    void endLine();
    string toString() const;


  private:
    typedef Writer super;

    static const char *START;
    static const char END = ')';

    static const char SECTION_START = '{';
    static const char SECTION_END = '}';

    static const char CONTENT_START = '(';
    static const char CONTENT_END = ')';

    static const char VALUE_START = '\'';
    static const char VALUE_END = '\'';
    static const char VALUE_SEPARATOR = ':';
    static const char FIELD_SEPARATOR = ',';

    ostringstream *stream;

    void writePlain(const string &str);
};

} /* namespace common */
} /* namespace dtests */

#endif /* DTESTS_COMMON_UPSTREAMWRITER_H */

