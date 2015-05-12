#ifndef __BROKER_AGENT_H__
#define __BROKER_AGENT_H__



#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <stdexcept>

#include <qpid/messaging/Address.h>
#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Session.h>

#include <qpid/types/Variant.h>
#include <qpid/types/Exception.h>

#include <boost/algorithm/string.hpp>

#include "formatter/Formatter.h"


class BrokerAgent
{
public:
  BrokerAgent();
  BrokerAgent(const std::string& brokerUrl, const std::string& connectionOptions);
  ~BrokerAgent();
  // set options afterwards
  void setEndpoint(const std::string& brokerUrl, const std::string& connectionOptions);

  /* Controls */
  void open();
  void close();
  void reset();

  /* status */
  bool isOpen();

  /* query (OFFLINE) */
  const qpid::types::Variant & query();
  const qpid::types::Variant & query(const char * queryData);
  const qpid::types::Variant & query(const std::string& queryData);

  /* DATA COMM API */
  // fetch all objects defined by BrokerAgent::brokerObjects
  // saves data to DB, e.g. Sync DataBase
  qpid::types::Variant::Map & fetchAllBrokerObjects();
  // fetch object by _class_name
  // returns copy of fetched data
  qpid::types::Variant::Map fetchObjects(std::string);

  // call qmf method, returns copy of fetched data
  qpid::types::Variant::Map callQmfMethod(qpid::types::Variant::Map&);

  // Reference VARIANT-VOID type (e.g. None)
  const static qpid::types::Variant VARVOID;

  /* Broker Model data */
  // broker object _class_name(s) to fetch
  const static std::string brokerObjects[];
  // auto computed length of above
  const static unsigned int brokerObjectsCount;

private:
  // state data
  std::string reply_to;
  unsigned long correlation_pool;

  // options data
  std::string brokerUrl;
  std::string connectionOptions;

  // QPID Messaging comm objects
  qpid::messaging::Connection connection;
  qpid::messaging::Session session;
  qpid::messaging::Sender sender;
  qpid::messaging::Receiver receiver;

  // Database Objects
  qpid::types::Variant::Map db;
  // only Variant typed Database (same as above)
  qpid::types::Variant vardb;

  /** COMM API **/
  std::string sendRequest(std::string, qpid::types::Variant::Map&, std::string);
  qpid::messaging::Message receive(std::string);

  /* helpers */
  std::string getRandomString();
  std::string getNextCorrelationId();

  /* maps utils */

  // search through map for name of map's object
  std::string extractObjectName(const qpid::types::Variant::Map&);

  // response is VarList<VarMap> go through object's map in response and 
  // transfer them into one MAP addressed with object names
  qpid::types::Variant::Map reMapResponse(qpid::types::Variant&);
  // varinant of above with List<VarMap>
  qpid::types::Variant::Map reMapResponse(qpid::types::Variant::List&);

  // selects the submap by the supplied key and elevate all values from sub map to upper map
  // expects Map<VarMap> to exists by key, otherwise key is copied unchanged
  qpid::types::Variant::Map elevateSubMap(std::string, const qpid::types::Variant::Map &);

  /** STATEFUL API **/

  /* searchMap */
  // search through BrokerAgent's database, by the supplied key (recursively)
  const qpid::types::Variant & searchMap(std::string);
  // as above, but search through supplied map
  const qpid::types::Variant & searchMap(std::string, const qpid::types::Variant::Map &);
  // as above, added recursion modifier
  const qpid::types::Variant & searchMap(std::string, const qpid::types::Variant::Map &, bool);
  // search consecutively by the keys in vector, in structured maps, one key per level
  // search in BrokerAgent's database
  const qpid::types::Variant & searchMap(std::vector<std::string>);
  // as above, but search in supplied map
  const qpid::types::Variant & searchMap(std::vector<std::string>, const qpid::types::Variant::Map &);


public:
  class NoResponseAvailable : public std::runtime_error {
  public:
    NoResponseAvailable() : std::runtime_error("No QMF valid response is available from broker") { }
  };
};

#endif
