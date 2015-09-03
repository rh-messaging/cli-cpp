
#include "BrokerAgent.h"

// setting reference VARIANT-VOID object as default empty return type
const qpid::types::Variant
BrokerAgent::VARVOID = qpid::types::Variant();

// setting broker objects we care about
//const std::string BrokerAgent::brokerObjects[] = { "exchange", "queue" };

const std::string BrokerAgent::brokerObjects[] = { "connection", "session", 
    "subscription", "link", "queue", "exchange", "binding", "topic", "system",
    "broker", "managementsetupstate", "memory", "bridge", "incoming", "outgoing",
    "domain", "agent", "vhost", "store", "journal", "acl", "queuepolicy", "topicpolicy" };

// coputing objects count
const unsigned int BrokerAgent::brokerObjectsCount = sizeof(BrokerAgent::brokerObjects) / sizeof(BrokerAgent::brokerObjects[0]);


BrokerAgent::BrokerAgent()
{
  this->vardb = qpid::types::Variant(db);
  this->reset();
}

BrokerAgent::BrokerAgent(const std::string& brokerUrl, const std::string& connectionOptions)
  : brokerUrl(brokerUrl), connectionOptions(connectionOptions)
{
  this->reset();
}

BrokerAgent::~BrokerAgent()
{
  this->close();
}

void 
BrokerAgent::setEndpoint(const std::string& brokerUrl, const std::string& connectionOptions)
{
  this->brokerUrl = brokerUrl;
  this->connectionOptions = connectionOptions;
}


void 
BrokerAgent::open()
{
  try {
    this->connection =  qpid::messaging::Connection(this->brokerUrl, this->connectionOptions);
    this->connection.open();
    this->session = this->connection.createSession("QMF Broker Agent");


    std::stringstream tmp;
    tmp << "qmf.default.topic/direct.";
    tmp << this->getRandomString();
    tmp << ";{node:{type:topic}}"; //, link:{x-declare:{auto-delete:True,exclusive:True}}}";

    this->reply_to = tmp.str();
    this->receiver = this->session.createReceiver(this->reply_to);
    this->receiver.setCapacity(10);
    this->sender = this->session.createSender("qmf.default.direct");

  } catch (qpid::messaging::MessagingException & exc) {
    std::cerr << exc.what() << std::endl;
    this->reset();
  }

  return;
}

void
BrokerAgent::close()
{
  if (this->isOpen()) {
    try {
      this->connection.close();
    } catch (qpid::messaging::MessagingException & exc) {
    }
  }
}

void
BrokerAgent::reset()
{
  this->close();
  this->correlation_pool = 1;
}

bool
BrokerAgent::isOpen()
{
  bool stat = true;
  stat &= this->connection.isOpen();
  return stat;
}

const qpid::types::Variant &
BrokerAgent::query()
{
  return this->vardb;
}

const qpid::types::Variant &
BrokerAgent::query(const char * queryData)
{
  std::string tmpQuery(queryData);
  return this->query(tmpQuery);
}

const qpid::types::Variant &
BrokerAgent::query(const std::string& queryData)
{
  std::vector<std::string> queryPath;
  boost::split(queryPath, queryData, boost::is_any_of("/"));

  return this->searchMap(queryPath);
}


  /** PRIVATE **/

std::string
BrokerAgent::getRandomString()
{
  return qpid::types::Uuid(true).str(); 
}

std::string
BrokerAgent::getNextCorrelationId()
{
  std::stringstream tmp;
  tmp << this->correlation_pool++;
  return tmp.str();
}

qpid::types::Variant::Map &
BrokerAgent::fetchAllBrokerObjects()
{
  this->db.clear();
  //::Formatter formatter;
  this->vardb = qpid::types::Variant();

  for (unsigned int indx = 0; indx < BrokerAgent::brokerObjectsCount; indx++) {
    this->db[BrokerAgent::brokerObjects[indx]] = this->fetchObjects(BrokerAgent::brokerObjects[indx]);
    //::Formatter formatter;
    //std::cerr << formatter.formatVariant(this->db[BrokerAgent::brokerObjects[indx]]) << std::endl;
   // std::cerr << "OBJ" << BrokerAgent::brokerObjects[indx] << std::endl;
  }
  this->vardb = qpid::types::Variant(db);
  return this->db;
}

qpid::types::Variant::Map
BrokerAgent::fetchObjects(std::string class_name)
{

  qpid::types::Variant::Map query;
  query["_what"] = "OBJECT";
  qpid::types::Variant::Map schema_id;
  //schema_id["_package_name"] = "org.apache.qpid.broker";
  schema_id["_class_name"] = class_name;
  query["_schema_id"] = schema_id;
  this->sendRequest("_query_request", query, "broker");

  qpid::messaging::Message response;
  qpid::types::Variant::Map result = qpid::types::Variant::Map();
  qpid::types::Variant::List result_list;
  while (true) {
    try {
      response = this->receive("_query_response");
    } catch (BrokerAgent::NoResponseAvailable & exc) {
      std::cerr << exc.what() << std::endl;
      return qpid::types::Variant::Map();
    }

    qpid::types::Variant response_content;
    qpid::types::Variant::List response_list;
    if (response.getContentType() == "amqp/list") {
#ifdef NGETCONTENTOBJECT
      decode(response, response_list);
#else
      response_content = response.getContentObject();
      response_list = response_content.asList();
#endif
      //::Formatter formatter;
      //std::cerr << formatter.formatList(response_list) << std::endl;
      result_list.splice(result_list.end(), response_list);
    }
    if (this->receiver.getAvailable() == 0) {
      break;
    }
  }
  result = reMapResponse(result_list);
  return result;
}

qpid::types::Variant::Map BrokerAgent::callQmfMethod(qpid::types::Variant::Map& query)
{
  this->sendRequest("_method_request", query, "broker");

  qpid::messaging::Message response;
  try {
    response = this->receive("_method_response");
  } catch (BrokerAgent::NoResponseAvailable & exc) {
    std::cerr << exc.what() << std::endl;
    return qpid::types::Variant::Map();
  }

  qpid::types::Variant response_content;
  qpid::types::Variant::Map result;
  if (response.getContentType() == "amqp/map") {
#ifdef NGETCONTENTOBJECT
    decode(response, result);
#else
    response_content = response.getContentObject();
    result = response_content.asMap();
#endif
  }
  return this->elevateSubMap("_arguments", result);
}

std::string
BrokerAgent::sendRequest(std::string opcode, qpid::types::Variant::Map& content, std::string qmf_class)
{
  qpid::messaging::Message message;
  //qpid::types::Variant::Map props;
  message.setProperty("method", "request");
  message.setProperty("qmf.opcode", opcode);
  message.setProperty("x-amqp-0-10.app-id", "qmf2");


#ifdef NGETCONTENTOBJECT
                message.setContentType("amqp/map");
                encode(content, message);
#else
                message.getContentObject() = content;
#endif

  message.setReplyTo(this->reply_to);
  std::string currentCorrelatioId = this->getNextCorrelationId();
  message.setCorrelationId(currentCorrelatioId);
  message.setSubject(qmf_class);
/*
  ::Formatter formatter;
  formatter.printMessageAsDict(message);
  std::cout << std::endl;
*/

  this->sender.send(message);
  return currentCorrelatioId;

}

qpid::messaging::Message
BrokerAgent::receive(std::string opcode)
{
  qpid::messaging::Message message;
  while (true) {
    try {
      message = this->receiver.fetch(qpid::messaging::Duration::SECOND*10);
    } catch (qpid::messaging::NoMessageAvailable & exc) {
      std::cerr << exc.what() << std::endl;
      throw BrokerAgent::NoResponseAvailable();
      break;
    }
/*
    ::Formatter formatter;
    std::cout << "YYY: ";
    formatter.printMessageAsDict(message);
*/
    const qpid::types::Variant::Map & msgProps = message.getProperties();
    qpid::types::Variant::Map::const_iterator map_iter;
    map_iter = msgProps.find("qmf.opcode");
    if (map_iter == msgProps.end()) {
      continue;
    }
    const qpid::types::Variant & qmfOpcode = map_iter->second;
    if( (qmfOpcode.asString() == opcode) ||
        (qmfOpcode.asString() == "_exception") ){
      break; // finally a response!
    }
  }

  return message;
}


std::string
BrokerAgent::extractObjectName(const qpid::types::Variant::Map& in_map)
{
  const std::string labels[] = { "name", "address", "_object_id/_object_name" };
  const unsigned int labels_cnt = sizeof(labels)/sizeof(labels[0]);
  //qpid::types::Variant obj_name;
  std::string obj_name = "";
  for (unsigned int indx = 0; indx < labels_cnt; indx++) {
    try {
      //obj_name = in_map.at("_values").asMap().at(labels[indx]);
      std::vector<std::string> path;
      boost::split(path, labels[indx], boost::is_any_of("/"));
      const qpid::types::Variant & tmp_result = this->searchMap(path, in_map);
      //std::cout << "TYPE " << tmp_result.getType() << std::endl;
      if (tmp_result.getType() == qpid::types::VAR_STRING){
        obj_name = tmp_result.asString();
      }
      if (obj_name.empty() == false) {
        break;
      }
    } catch (std::exception exp) {
      std::cerr << exp.what() << std::endl;
    }
  }

  //return obj_name.asString();
  return obj_name;
}

qpid::types::Variant::Map
BrokerAgent::reMapResponse(qpid::types::Variant::List& in_data)
{

  qpid::types::Variant::Map result;

  qpid::types::Variant::List::const_iterator list_iter;
  for (list_iter = in_data.begin(); list_iter != in_data.end(); list_iter++) {
    if (list_iter->getType() == qpid::types::VAR_MAP) {
      std::string obj_name = "";
      const qpid::types::Variant::Map & tmp_map = list_iter->asMap();
      //::Formatter formatter;
      //std::cout << "ZZZ " << formatter.formatMap(tmp_map) << std::endl;
      obj_name = this->extractObjectName(tmp_map);
      result[obj_name] = this->elevateSubMap("_values", tmp_map);
    }
  }

  return result;

}

qpid::types::Variant::Map
BrokerAgent::reMapResponse(qpid::types::Variant& in_data)
{
  if (in_data.getType() != qpid::types::VAR_LIST) {
    return qpid::types::Variant::Map();
  }
  qpid::types::Variant::List src_list;
  src_list = in_data.asList();

  return this->reMapResponse(src_list);
}


qpid::types::Variant::Map
BrokerAgent::elevateSubMap(std::string key, const qpid::types::Variant::Map & in_data)
{
  qpid::types::Variant::Map result;
  qpid::types::Variant::Map::const_iterator map_iter;

  for (map_iter = in_data.begin(); map_iter != in_data.end(); map_iter++) {
    // key match
    if (key.compare(map_iter->first) == 0) {
      // if submap exists then elevate values
      if (map_iter->second.getType() == qpid::types::VAR_MAP) {
        const qpid::types::Variant::Map & inner_map = map_iter->second.asMap();
        qpid::types::Variant::Map::const_iterator inner_map_iter;
        for (inner_map_iter = inner_map.begin(); inner_map_iter != inner_map.end(); inner_map_iter++) {
          result[inner_map_iter->first] = inner_map_iter->second;
        }
      // no map to elevate -> copy unchanged
      } else {
        result[map_iter->first] = map_iter->second;
      }
    // no key match -> copy
    } else {
      result[map_iter->first] = map_iter->second;
    }
  }
  //std::cerr << "ELEVATE" << std::endl;
  return result;
}

const qpid::types::Variant &
BrokerAgent::searchMap(std::string key)
{
  return this->searchMap(key, this->db);
}

const qpid::types::Variant &
BrokerAgent::searchMap(std::string key, const qpid::types::Variant::Map & in_data)
{
  return this->searchMap(key, in_data, true);
}

const qpid::types::Variant &
BrokerAgent::searchMap(std::string key, const qpid::types::Variant::Map & in_data, bool recursive)
{
  //std::cout << "SEARCH " << key << std::endl;
  qpid::types::Variant::Map::const_iterator map_iter;
  for (map_iter = in_data.begin(); map_iter != in_data.end(); map_iter++) {
    //std::cout << ".. KEY " << map_iter->first << std::endl;
    if (key.compare(map_iter->first) == 0) {
      //std::cout << "== FOUND " << map_iter->second.asString() << std::endl;
      return map_iter->second;
    }
  }
  if (recursive == true) {
    for (map_iter = in_data.begin(); map_iter != in_data.end(); map_iter++) {
      if (map_iter->second.getType() == qpid::types::VAR_MAP) {
        const qpid::types::Variant::Map & tmp_map = map_iter->second.asMap();
        const qpid::types::Variant & tmp_result = this->searchMap(key, tmp_map);
        //std::cout << "STYPE " << tmp_result.getType() << std::endl;
        if (tmp_result.getType() != qpid::types::VAR_VOID) { // if result is not VOID, then return
          return tmp_result;
        }
        //otherwise continue searching
      }
    }
  }

  //std::cout << "!! NFOUND " << this->VARVOID.getType() << std::endl;
  return this->VARVOID;
}

const qpid::types::Variant &
BrokerAgent::searchMap(std::vector<std::string> path)
{
  return this->searchMap(path, this->db);
}

const qpid::types::Variant &
BrokerAgent::searchMap(std::vector<std::string> path, const qpid::types::Variant::Map & in_data)
{

  const qpid::types::Variant & subVariant = this->searchMap(*(path.begin()), in_data);
  path.erase(path.begin());
  if (path.empty() == true) {
    return subVariant;
  }
  if (subVariant.getType() == qpid::types::VAR_MAP) {
    const qpid::types::Variant & resultVariant = this->searchMap(path, subVariant.asMap());
    return resultVariant;
  }
  return this->VARVOID;

}


/* -- EOF BrokerAgent.cpp -- */
