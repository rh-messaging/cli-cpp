/* 
 * File:   ContentTypes.h
 * Author: opiske
 *
 * Created on May 5, 2015, 3:32 PM
 */

#ifndef CONTENTTYPES_H
#define	CONTENTTYPES_H

#include <string>

using std::string;

class ContentType {
public:
    static const char *TEXT_PLAIN;
    
    
private:
    
    ContentType();
    ContentType(const ContentType& orig);
    virtual ~ContentType();

};

#endif	/* CONTENTTYPES_H */

