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
    virtual string decodeValue(bool) const;
    virtual void decodeValue(Writer *writer, bool) const;
    
    virtual string decodeValue(const string &) const;
    virtual void decodeValue(Writer *writer, const string &) const;
    
    template <typename T> 
    string decodeValue(T number) const {
        ostringstream ret;
  
        ret << number;
        
        return string(ret.str());
    }
    
    
    virtual string decodeValue(float number) const;
    virtual void decodeValue(Writer *writer, float number) const;
    

    template <typename T, typename Y>
    void decodeValue(Writer *writer, const map<T, Y> &valuesMap) const {
        
        if (valuesMap.size() == 0) {
            return;
        }
        
        writer->startMap();
        for(typename map<T, Y>::const_iterator iterator = valuesMap.begin(); 
                iterator != valuesMap.end(); iterator++) 
        {
            if (iterator != valuesMap.begin()) {
                writer->endField();
            }
            
            KeyValue kv = KeyValue(iterator->first, 
                    static_cast<T>(iterator->second));
            
            writer->write(kv, false);
            
        }
        writer->endMap();
    }
    
    template <typename T>
    void decodeValue(Writer *writer, const list<T> &inputList) {
        
        if (inputList.size() == 0) {
            return 0;
        }
        
        writer->startList();
        for(typename list<T>::const_iterator iterator = inputList.begin(); 
                iterator != inputList.end(); 
                iterator++)
        {
             decodeValue(writer, static_cast<T>(*iterator));
        }
        writer->endList();
    }
   
private:
    string quoteStringEscape(const string& a) const;
       

};

#endif	/* ABSTRACTDECODER_H */

