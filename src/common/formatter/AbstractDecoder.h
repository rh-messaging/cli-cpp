/* 
 * File:   AbstractDecoder.h
 * Author: opiske
 *
 * Created on May 5, 2015, 1:22 PM
 */

#ifndef ABSTRACTDECODER_H
#define	ABSTRACTDECODER_H

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>


#include "KeyValue.h"
#include "Writer.h"

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
    
    virtual void decodeHeader(Writer *writer) const = 0;
    virtual void decodeProperties(Writer *writer) const = 0;
    virtual void decodeContent(Writer *writer) const = 0;
    
protected:
    string decodeValue(bool) const;
    void decodeValue(Writer *writer, bool) const;
    
    string decodeValue(const string &) const;
    void decodeValue(Writer *writer, const string &) const;
    
    template <typename T> 
    string decodeValue(T number) const {
        ostringstream ret;
  
        ret << number;
        
        return string(ret.str());
    }
    
    
    string decodeValue(float number) const;
    void decodeValue(Writer *writer, float number) const;
    

    template <typename T, typename Y>
    void decodeValue(Writer *writer, const map<T, Y> &valuesMap) const {
        
        writer->startMap();
        for(typename map<T, Y>::const_iterator iterator = valuesMap.begin(); 
                iterator != valuesMap.end(); iterator++) 
        {
            writer->write(iterator->first);
            decodeValue(writer, iterator->second);
           
        }
        writer->endMap();
    }
    
    template <typename T>
    void decodeValue(Writer *writer, const list<T> &inputList) {
        
        writer->startList();
        for(typename list<T>::const_iterator iterator = inputList.begin(); 
                iterator != inputList.end(); 
                iterator++)
        {
             decodeValue(writer, *iterator);
        }
        writer->endList();
    }
   
private:
    string quoteStringEscape(const string& a) const;
       

};

#endif	/* ABSTRACTDECODER_H */

