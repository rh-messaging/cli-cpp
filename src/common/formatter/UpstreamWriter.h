/* 
 * File:   StringWriter.h
 * Author: opiske
 *
 * Created on May 6, 2015, 4:13 PM
 */

#ifndef STRINGWRITER_H
#define	STRINGWRITER_H

#include <string>
#include <sstream>

#include "KeyValue.h"
#include "Writer.h"

using std::string;
using std::ostringstream;

class UpstreamWriter: public Writer {
public:
    UpstreamWriter(ostringstream *stream);
    virtual ~UpstreamWriter();
    
    void start();
    void end();
    
    // NO-OP
    void startHeader() {}
    
    // NO-OP
    void endHeader() {}
    
    void startProperties();
    
    // NO-OP
    void endProperties() {};
    
    void startContent();
    void endContent();
    
    void startMap();
    void endMap();
    
    void startList();
    void endList();
    
    void endField();
    
    void write(const string &str);
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
    static const char VALUE_SEPARATOR = ':' ;
    static const char FIELD_SEPARATOR = ',' ;
    
    ostringstream *stream;
    
    void writePlain(const string &str);
};

#endif	/* STRINGWRITER_H */

