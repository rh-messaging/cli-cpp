/* QMF v2 C++ console reader */


#include "OptionParser.h"
#include "formatter/Formatter.h"
#include "BrokerAgent.h"
#include "Utils.h"

#include <iostream>
#include <ctime>
#include <cstdlib>

#include <map>
#include <string>
#include <vector>

#include <qpid/types/Variant.h>

class Options : public OptionParser
{
  public:
    const static std::string QUERY_ALL;
    std::string broker;
    std::string conn_options;
    std::string log_data_fmt;
    std::string query_data;
    int repeat_cnt;
    int repeat_duration;
    int close_sleep;

    Options()
      : OptionParser("Usage: qmf2_list_objects [options]",
        "Query Qpid broker and get speficied QMF data"),
        broker("localhost"),
        conn_options("{}"),
        log_data_fmt("pynative"),
        query_data(Options::QUERY_ALL),
        repeat_cnt(1),
        repeat_duration(0),
        close_sleep(0)
    {
      add("broker,b", broker, "connect to specified BROKER url");
      add("connection-options", conn_options, "connection options string in the form {name1:value,name2:value2}");
      add("log-data-fmt", log_data_fmt, "Data report format (pynative/formatter/pprint)");
      add("query-data", query_data, "Query specific QMF data (None for all)");
      add("repeat-cnt", repeat_cnt, "Number of QMF queries to be performed");
      add("repeat-duration", repeat_duration, "QMF queries repeat duration (to be used together with repeat-cnt)");
      add("close_sleep", close_sleep, "Sleep before sender/receiver/session/connection.close()");
    }

};

const std::string Options::QUERY_ALL = std::string("__QUERY_ALL__");

class QMF2ListObjects
{
  public:
    QMF2ListObjects(const Options&);
    ~QMF2ListObjects();
    int main();
  private:
    const Options& opts;
    ::Formatter formatter;
    ::BrokerAgent agent;

};

QMF2ListObjects::QMF2ListObjects(const Options& in_opts)
  : opts(in_opts)
{
  return;
}

QMF2ListObjects::~QMF2ListObjects()
{
  return;
}

int QMF2ListObjects::main()
{
  this->agent = ::BrokerAgent(this->opts.broker, this->opts.conn_options);
  this->agent.open();
  //std::string queryString = "";
  //std::cout << this->formatter.formatVariant(this->agent.query(queryString)) << std::endl;
  /*
  if (this->agent.isOpen()) {
    std::cout << "Open";
  } else {
    std::cout << "Closed";
  }
  std::cout << std::endl;
  */

  //std::cout << this->formatter.formatMap(this->agent.fetchAllBrokerObjects()) << std::endl << std::endl;

  //std::time_t start = std::time(0);


  if (this->agent.isOpen() == false) {
    std::cerr << "Agent failed to establish communication channels to broker" << std::endl;
    return 1;
  }

  double ts = get_time();

  for (int round = 0; round < this->opts.repeat_cnt; round++) {
    this->agent.fetchAllBrokerObjects();
    if (this->opts.query_data == Options::QUERY_ALL) {
      std::cout << this->formatter.formatVariant(this->agent.query()) << std::endl;
    } else {
      std::cout << this->formatter.formatVariant(this->agent.query(this->opts.query_data)) << std::endl;
    }

    // WAIT
    sleep4next(ts, this->opts.repeat_cnt, this->opts.repeat_duration, round+1);
  }

  //std::cout << this->formatter.formatMap(this->agent.fetchObjects("queue")) << std::endl << std::endl;
  //std::cout << this->formatter.formatMap(this->agent.fetchObjects("exchange")) << std::endl << std::endl;
  
 // std::string in;
 // std::cin >> in;

  this->agent.close();
  return 0;
}





int main(int argc, char ** argv)
{
  Options options;
  QMF2ListObjects qmfc(options);

  if (!options.parse(argc, argv)) {
    return(2);
  }

  int ecode = qmfc.main();
  return(ecode);
}





