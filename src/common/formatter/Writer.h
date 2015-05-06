/* 
 * File:   Writer.h
 * Author: opiske
 *
 * Created on May 5, 2015, 6:26 PM
 */

#ifndef WRITER_H
#define	WRITER_H

#include <string>

#include "KeyValue.h"

using std::string;

class Writer {
public:
    static const char *BOGUS_FORMAT_MESSAGE;
    
    Writer();
    virtual ~Writer();
    
    virtual void startHeader() = 0;
    virtual void endHeader() = 0;
    
    virtual void startProperties() = 0;
    virtual void endProperties() = 0;
    
    virtual void startContent() = 0;
    virtual void endContent() = 0;
    
    virtual void startMap() = 0;
    virtual void endMap() = 0;
    
    virtual void startList() = 0;
    virtual void endList() = 0;
    
    virtual void write(const string &str) = 0;
    virtual void write(const KeyValue &keyValue) = 0;
        
    virtual void writeBogus(const string &str) = 0;
    
private:
    
};

#endif	/* WRITER_H */

