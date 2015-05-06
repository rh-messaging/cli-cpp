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
    void printMessage(const AbstractDecoder *decoder, Writer *writer) const;

    /* Print message "python dictionary style" to be easily evaluated
     * by python code. Print all available headers.
     */
    // void printMessageAsDict(const AbstractDecoder *decoder, Writer *writer) const;

    /**
     * Print message processing statistics
     * @param decoder
     */
    // void printStatistics(const AbstractDecoder *decoder, Writer *writer) const;

    /* format Variant into proper formatted string using above formatting methods */
    // string formatVariant(const qpid::types::Variant);

    /* recursive python map formatter {'key': 'value' } */
    // std::string formatMap(const qpid::types::Variant::Map&);
protected:
};

#endif	/* ABSTRACTFORMATTER_H */

