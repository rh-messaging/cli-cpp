
#include "Formatter.h"

#include <iomanip>
#include <vector>

void Formatter::printMessage(qpid::messaging::Message& msg, bool verbose)
{
    std::cout << "Message(";
    if (verbose) {

        if (msg.getRedelivered())
            std::cout << "redelivered='True', ";
        if (msg.getReplyTo())
            std::cout << "reply_to='" << msg.getReplyTo().getName() << "', ";
        if (msg.getSubject() != "")
            std::cout << "subject='" << msg.getSubject() << "', ";
        if (msg.getContentType() != "")
            std::cout << "content_type='" << msg.getContentType() << "', ";
        if (msg.getMessageId() != "")
            std::cout << "id='%s'" << msg.getMessageId() << "', ";
        if (msg.getUserId() != "")
            std::cout << "user_id='" << msg.getUserId() << "', ";
        if (msg.getCorrelationId() != "")
            std::cout << "correlation_id='" << msg.getCorrelationId() << "', ";
        if (msg.getPriority())
            std::cout << "priority='" << (int)msg.getPriority() << "', ";
        if (msg.getDurable())
            std::cout << "durable='True', ";
        if (msg.getTtl().getMilliseconds())
            std::cout << "ttl='%s'" << msg.getTtl().getMilliseconds() << "', ";
        if (msg.getContentSize())
            std::cout << "size=" << formatContentSize(msg) << ", ";
    }
    std::cout << "properties={";
    qpid::types::Variant::Map& props = msg.getProperties();
    for(qpid::types::Variant::Map::const_iterator iterator = props.begin(); iterator != props.end(); iterator++) {
        if (iterator != props.begin()) std::cout << ", ";
        std::cout << "'" << iterator->first << "': '" << iterator->second << "'";
    }
    std::cout << "}, content='" ;
    if (isMapMsg(msg)) {
        qpid::types::Variant::Map map = getMsgAsMap(msg);
        std::cout << map;
    } else {
        std::cout << msg.getContent();
    }
    std::cout << "')" << std::endl;
}



void Formatter::printStatistics(qpid::types::Variant::Map& statistics) {
  std::cout << "STATS " << formatMap(statistics) << std::endl;
}

void Formatter::printMessageAsDict(qpid::messaging::Message& msg) {
std::ostringstream int_result;
int_result << "{"
  << "'redelivered': " << formatBool(msg.getRedelivered()) << ", "
  << "'reply_to': " << formatString(msg.getReplyTo().str()) << ", "
  << "'subject': " << formatString(msg.getSubject()) << ", "
  << "'content_type': " << formatString(msg.getContentType()) << ", "
  << "'id': " << formatString(msg.getMessageId()) << ", "
  << "'user_id': " << formatString(msg.getUserId()) << ", "
  << "'correlation_id': " << formatString(msg.getCorrelationId()) << ", "
  << "'priority': " << formatPriority(msg.getPriority()) << ", "
  << "'durable': " << formatBool(msg.getDurable()) << ", "
  << "'ttl': " << formatTTL(msg.getTtl().getMilliseconds()) << ", "
// BZ1023083 fixed in 0.22-23
  << "'size': " << formatContentSize(msg) << ", "

  << "'properties': " << formatMap(msg.getProperties()) << ", "

  << "'content': ";
  if (isMapMsg(msg)) {
      qpid::types::Variant::Map map = getMsgAsMap(msg);
      int_result << formatMap(map);
  } else if (isListMsg(msg)) {
      qpid::types::Variant::List list = getMsgAsList(msg);
      int_result << formatList(list);
  } else {
      int_result << formatString(msg.getContent());
  }
  int_result << "}" << std::endl;

  std::cout << int_result.str();
}


/* -------------- Formating functions ----------- */

std::string Formatter::formatBool(bool in_data) {
  if (in_data == true) {
    return std::string("True");
  }
  return std::string("False");
}

std::string Formatter::formatString(const std::string & in_data) {
  if (in_data.empty()) {
    return std::string("None");
  }
  std::string int_result = std::string("'");
  int_result.append(quoteStringEscape(in_data));
  int_result.append("'");

  return int_result;
}

std::string Formatter::formatInt(short int in_data) {
  return formatInt((long long int)in_data);
}

std::string Formatter::formatInt(int in_data) {
  return formatInt((long long int)in_data);
}

std::string Formatter::formatInt(long int in_data) {
  return formatInt((long long int)in_data);
}

std::string Formatter::formatInt(long long int in_data) {
  std::ostringstream int_result;
  int_result << in_data;
  return std::string(int_result.str());
}

std::string Formatter::formatInt(unsigned short int in_data) {
  return formatInt((unsigned long long int)in_data);
}

std::string Formatter::formatInt(unsigned int in_data) {
  return formatInt((unsigned long long int)in_data);
}

std::string Formatter::formatInt(unsigned long int in_data) {
  return formatInt((unsigned long long int)in_data);
}

std::string Formatter::formatInt(unsigned long long int in_data) {
  std::ostringstream int_result;
  int_result << in_data;
  return std::string(int_result.str());
}

std::string Formatter::formatFloat(float in_data) {
  std::ostringstream int_result;
  int_result << std::scientific << in_data;
  return std::string(int_result.str());
}



std::string Formatter::formatMap(const qpid::types::Variant::Map& in_data) {
  std::string int_result = std::string("{");

  for(qpid::types::Variant::Map::const_iterator iterator = in_data.begin(); iterator != in_data.end(); iterator++) {
    if (iterator != in_data.begin()) {
      int_result.append(", ");
    }
    int_result.append("'").append(iterator->first).append("': ").append(formatVariant(iterator->second));
  }
  int_result.append("}");
  return int_result;
}

std::string Formatter::formatList(const qpid::types::Variant::List& in_data) {
  std::string int_result = "[";
  for(qpid::types::Variant::List::const_iterator iterator = in_data.begin(); iterator != in_data.end(); iterator++) {
    if (iterator != in_data.begin()) {
      int_result.append(", ");
    }
    int_result.append(formatVariant(*iterator));
  }
  int_result.append("]");
  return int_result;
}



std::string Formatter::formatPriority(int in_data) {
  /*
  if (in_data == 0) {
    return std::string("None");
  }
  */
  return formatInt(in_data);
}

std::string Formatter::formatTTL(int in_data) {
  float int_result = float(in_data) / 1000.0;
  return formatFloat(int_result);
}


std::string Formatter::formatContentSize(qpid::messaging::Message & in_msg) {
  std::string int_result = "";
  int_result.assign(formatInt(in_msg.getContentSize()));

  if (in_msg.getContentSize() == 0) {
    if (in_msg.getContentType() != "text/plain") {
      int_result.assign("None");
    }
  }
  return int_result;
}

std::string Formatter::formatBogus(const qpid::types::Variant & in_data) {
  std::ostringstream int_result;
  int_result << "!!!FORMATTING ERROR, BUG!!!";
  int_result << in_data.asString();
  return std::string(int_result.str());
}


std::string Formatter::formatVariant(const qpid::types::Variant in_data) {
  std::string int_result = "";
  qpid::types::VariantType varType = in_data.getType();
  if (varType == qpid::types::VAR_STRING) {
  //if (varType == qpid::types::VAR_STRING) {
    int_result.append(formatString(in_data.asString()));
  } else if (varType == qpid::types::VAR_BOOL) {
    int_result.append(formatBool(in_data.asBool()));
  } else if (varType == qpid::types::VAR_FLOAT) {
    int_result.append(formatFloat(in_data.asFloat()));
  } else if (varType == qpid::types::VAR_DOUBLE) {
    int_result.append(formatFloat(in_data.asDouble()));
  } else if (varType == qpid::types::VAR_UUID) {
    int_result.append(formatString(in_data.asString()));
  } else if (varType == qpid::types::VAR_VOID) {
    int_result.append("None");
  } else if (varType == qpid::types::VAR_MAP) {
    int_result.append(formatMap(in_data.asMap()));
  } else if (varType == qpid::types::VAR_LIST) {
    int_result.append(formatList(in_data.asList()));
  } else {
    // UInt, Int, and default section
    bool flagDone = false;
    std::vector<qpid::types::VariantType> varTypeInts;
    std::vector<qpid::types::VariantType> varTypeUInts;

    varTypeUInts.push_back(qpid::types::VAR_UINT8);
    varTypeUInts.push_back(qpid::types::VAR_UINT16);
    varTypeUInts.push_back(qpid::types::VAR_UINT32);
    varTypeUInts.push_back(qpid::types::VAR_UINT64);

    varTypeInts.push_back(qpid::types::VAR_INT8);
    varTypeInts.push_back(qpid::types::VAR_INT16);
    varTypeInts.push_back(qpid::types::VAR_INT32);
    varTypeInts.push_back(qpid::types::VAR_INT64);

    std::vector<qpid::types::VariantType>::iterator varTypeIterator;
    for (varTypeIterator = varTypeUInts.begin(); varTypeIterator != varTypeUInts.end(); varTypeIterator++) {
      if (varType == *varTypeIterator) {
        int_result.append(formatInt((unsigned long long int)in_data.asUint64()));
        flagDone = true;
        break;
      }
    }
    if (flagDone == false) {
      for (varTypeIterator = varTypeInts.begin(); varTypeIterator != varTypeInts.end(); varTypeIterator++) {
        if (varType == *varTypeIterator) {
          int_result.append(formatInt((long long int)in_data.asInt64()));
          flagDone = true;
          break;
        }
      }
    }

    if (flagDone == false) {
      int_result.append(formatBogus(in_data));
    }
  }
  return int_result;
}

/* -------------- Support message content related functions ----------- */

bool Formatter::isMapMsg(qpid::messaging::Message& msg) {
#ifdef NGETCONTENTOBJECT
  return(msg.getContentType() == "amqp/map");
#else
  return(msg.getContentObject().getType() == qpid::types::VAR_MAP);
#endif
}

bool Formatter::isListMsg(qpid::messaging::Message& msg) {
#ifdef NGETCONTENTOBJECT
  return(msg.getContentType() == "amqp/list");
#else
  return(msg.getContentObject().getType() == qpid::types::VAR_LIST);
#endif
}

qpid::types::Variant::Map Formatter::getMsgAsMap(qpid::messaging::Message& msg) {
  qpid::types::Variant::Map intMap;
#ifdef NGETCONTENTOBJECT
  decode(msg, intMap);
#else
  intMap = msg.getContentObject().asMap();
#endif
  return(intMap);
}
qpid::types::Variant::List Formatter::getMsgAsList(qpid::messaging::Message& msg) {
  qpid::types::Variant::List intList;
#ifdef NGETCONTENTOBJECT
  decode(msg, intList);
#else
  intList = msg.getContentObject().asList();
#endif
  return(intList);
}


/* -------------- Support formatting functions ----------- */
std::string Formatter::quoteStringEscape(const std::string& a) {
  char pattern = '\'';
  char range_low = 32;
  char range_high = 126;
  std::string int_result = "";
  for (std::string::const_iterator it = a.begin(); it != a.end(); it++) {
    if (*it < range_low || *it > range_high) {
      std::ostringstream tmp;
      /*
        \\\\ -- because of python eval escaping
       */
      tmp << "\\\\x" << std::setfill('0') << std::hex << std::setw(2) << int(*it);
      int_result.append(tmp.str());
    } else {
      if (*it == pattern) {
        int_result.push_back('\\');
      }
      int_result.push_back(*it);
    }
  }

  return int_result;
}




// eof

