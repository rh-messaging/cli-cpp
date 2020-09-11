/* 
 * File:   AbstractFormatter.h
 * Author: opiske
 *
 * Created on May 5, 2015, 12:50 PM
 */

#ifndef ABSTRACTFORMATTER_H
#define	ABSTRACTFORMATTER_H

#include <string>

#include "AbstractDecoder.h"
#include "Writer.h"

class AbstractFormatter {
public:
    AbstractFormatter();
    AbstractFormatter(const AbstractFormatter& orig);
    virtual ~AbstractFormatter();
        
    /**
     * Print message "upstream style" => 'Message(...)'
     * additional data could be aquired by 'bool verbose' parameter
     */
    virtual void printMessage(const AbstractDecoder *dec, Writer *wr) const = 0;

  
    /**
     * Print message processing statistics
     * @param decoder
     */
    
    template <typename T, typename Y>
    void printStatistics(const AbstractDecoder *decoder, Writer *writer, 
        const map<T, Y> &valuesMap) const 
    {
        writer->startStatistics();
        decoder->decodeStatistics(writer, valuesMap);
        writer->endStatistics();
    }

    /* format Variant into proper formatted string using above formatting methods */
    // string formatVariant(const qpid::types::Variant);

    /* recursive python map formatter {'key': 'value' } */
    // std::string formatMap(const qpid::types::Variant::Map&);
protected:
    
    
};

#endif	/* ABSTRACTFORMATTER_H */

