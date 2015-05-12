/* 
 * File:   DictFormatter.h
 * Author: opiske
 *
 * Created on May 11, 2015, 5:55 PM
 */

#ifndef DICTFORMATTER_H
#define	DICTFORMATTER_H

#include "AbstractFormatter.h"

class DictFormatter: public AbstractFormatter {
public:
    DictFormatter();
    DictFormatter(const DictFormatter& orig);
    virtual ~DictFormatter();
    
    void printMessage(const AbstractDecoder *decoder, Writer *writer) const;

private:
    typedef AbstractFormatter super;

};

#endif	/* DICTFORMATTER_H */

