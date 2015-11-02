/* 
 * File:   ContentTypes.h
 * Author: opiske
 *
 * Created on May 5, 2015, 3:32 PM
 */

#ifndef DTESTS_COMMON_CONTENTTYPES_H
#define DTESTS_COMMON_CONTENTTYPES_H

#include <string>

using std::string;

/**
 * RFC 2046-based list of supported content types (media types) in the 
 * messaging systems
 */
class ContentType {
  public:
    static const char *TEXT_PLAIN;
    static const char *AMQP_LIST;
    static const char *AMQP_MAP;

  private:

    ContentType();
    ContentType(const ContentType& orig);
    virtual ~ContentType();

};

#endif /* DTESTS_COMMON_CONTENTTYPES_H */

