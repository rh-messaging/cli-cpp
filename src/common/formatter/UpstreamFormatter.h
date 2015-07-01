/* 
 * File:   DictFormatter.h
 * Author: opiske
 *
 * Created on May 11, 2015, 5:55 PM
 */

#ifndef UPSTREAMFORMATTER_H
#define	UPSTREAMFORMATTER_H

#include "AbstractFormatter.h"

/**
 * Upstream-specific data formatter
 */
class UpstreamFormatter: public AbstractFormatter {
public:
    UpstreamFormatter();
    UpstreamFormatter(const UpstreamFormatter& orig);
    virtual ~UpstreamFormatter();
    
    void printMessage(const AbstractDecoder *decoder, Writer *writer) const;

private:
    typedef AbstractFormatter super;

};

#endif	/* DICTFORMATTER_H */

