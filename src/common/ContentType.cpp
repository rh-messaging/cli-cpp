/* 
 * File:   ContentTypes.cpp
 * Author: opiske
 * 
 * Created on May 5, 2015, 3:32 PM
 */

#include "ContentType.h"

using namespace dtests::common;

const char *ContentType::TEXT_PLAIN = "text/plain";
const char *ContentType::AMQP_LIST = "amqp/list";
const char *ContentType::AMQP_MAP = "amqp/map";

ContentType::ContentType()
{
}

ContentType::ContentType(const ContentType& orig)
{
}

ContentType::~ContentType()
{
}

