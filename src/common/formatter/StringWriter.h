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

class StringWriter: public Writer {
public:
    StringWriter(ostringstream *stream);
    virtual ~StringWriter();
    
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
    
    void write(const string &str);
    void write(const KeyValue &keyValue);
        
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
    
    static const char VALUE_START = '\'';
    static const char VALUE_END = '\'';
    static const char VALUE_SEPARATOR = ':' ;
    static const char FIELD_SEPARATOR = ',' ;
    
    ostringstream *stream;
};

#endif	/* STRINGWRITER_H */

