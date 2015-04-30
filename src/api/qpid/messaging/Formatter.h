#ifndef __FORMATER_CPP__
#define __FORMATER_CPP__


#include <qpid/messaging/Message.h>
#include <qpid/messaging/Address.h>
#include <qpid/types/Variant.h>

#include <iostream>
#include <sstream>


/* ** Recognized macros (defines)
 * NGETCONTENTOBJECT
 *   qpid::messaging::Message.getContentObject() not available (if defined)
 *   if defined then formatter will use decode for decoding list/map messages
 * */


struct Formatter
{
/* ** Printer functions 
 * directly prints messages onto std::cout
 * */

/* Print message "upstream style" => 'Message(...)'
 * additional data could be aquired by 'bool verbose' parameter
 * */
void printMessage(qpid::messaging::Message&, bool);

/* Print message "python dictionary style" to be easily evaluated
 * by python code. Print all available headers.
 * */
void printMessageAsDict(qpid::messaging::Message&);

void printStatistics(qpid::types::Variant::Map&);

/* ** Formaters functions 
 * used for unified formatting of certain datatypes
 * */

/* True / False */
std::string formatBool(bool);
/* "" -> None / <String> */
std::string formatString(const std::string&);
/* int */
std::string formatInt(short int);
std::string formatInt(int);
std::string formatInt(long int);
std::string formatInt(long long int);
std::string formatInt(unsigned short int);
std::string formatInt(unsigned int);
std::string formatInt(unsigned long int);
std::string formatInt(unsigned long long int);
/* float */
std::string formatFloat(float in_data);


/* recursive python map formatter {'key': 'value' } */
std::string formatMap(const qpid::types::Variant::Map&);
/* list variant into python dict */
std::string formatList(const qpid::types::Variant::List&);


/* 0 -> None / 1..N */
std::string formatPriority(int);
/* converts int milisecond TTL into float second TTL string */
std::string formatTTL(int);
/* returns a content size from message object or None if size == 0 and object is map/list */
std::string formatContentSize(qpid::messaging::Message&);

/* bogus input not triaged by format variant into any existing VAR_TYPE */
std::string formatBogus(const qpid::types::Variant&);


/* format Variant into proper formatted string using above formatting methods */
std::string formatVariant(const qpid::types::Variant);

/* Support message content related functions */
bool isMapMsg(qpid::messaging::Message& msg);
bool isListMsg(qpid::messaging::Message& msg);

qpid::types::Variant::Map getMsgAsMap(qpid::messaging::Message& msg);
qpid::types::Variant::List getMsgAsList(qpid::messaging::Message& msg);

std::string quoteStringEscape(const std::string&);

};

#endif

// eof
