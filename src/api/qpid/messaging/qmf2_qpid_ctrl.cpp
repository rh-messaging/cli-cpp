/* C+ QMF v2 methods call */

#include "OptionParser.h"
#include "BrokerAgent.h"

class Options : public OptionParser
{
  public:
    std::string broker;
    std::string conn_options;
    std::string qmf_class;
    std::string package;
    std::string id;
    std::string address;

    Options()
      : OptionParser("Usage: qmf2_qpid_ctrl [options] COMMAND ...",
        "Invoke the specified command."),
        broker("localhost"),
        conn_options("{}"),
        qmf_class("broker"),
        package("org.apache.qpid.broker"),
        id("amqp-broker"),
        address("qmf.default.direct/broker")
    {
      add("broker,b", broker, "connect to specified BROKER url");
      add("class,c", qmf_class, "class of object on which command is being invoked (default broker)");
      add("package,p", package, "package of object on which command is being invoked (default org.apache.qpid.broker)");
      add("id,i", id, "identifier of object on which command is being invoked (default amqp-broker)");
      add("address,a", address, "address to send commands to (default qmf.default.direct/broker)");
//      add("timeout,t", timeout, "timeout in seconds to wait for response before exiting (default 5)");
//      add("v", iverbose, "enable logging");
      add("connection-options", conn_options, "connection options string in the form {name1:value,name2:value2}");
    }

};

class QMF2QpidCtrl
{
  public:
    QMF2QpidCtrl(Options&);
    ~QMF2QpidCtrl();
    int main();
  private:
    Options& opts;
    ::Formatter formatter;
    ::BrokerAgent agent;

};

QMF2QpidCtrl::QMF2QpidCtrl(Options& in_opts)
  : opts(in_opts)
{
  return;
}

QMF2QpidCtrl::~QMF2QpidCtrl()
{
  return;
}

int QMF2QpidCtrl::main()
{
  this->agent = ::BrokerAgent(this->opts.broker, this->opts.conn_options);
  this->agent.open();

  if (this->agent.isOpen() == false) {
    std::cerr << "Agent failed to establish communication channels to broker" << std::endl;
    return 1;
  }

  std::vector<std::string>& args = this->opts.getArguments();
  std::vector<std::string> nameval;

  qpid::types::Variant::Map query;
  qpid::types::Variant::Map obj_id;
  qpid::types::Variant::Map arguments;

  for (unsigned int i = 1; i < args.size(); ++i) {
    boost::split(nameval, args[i], boost::is_any_of("="));
    arguments[nameval[0]] = nameval[1];
  }
  obj_id["_object_name"] = opts.package + ":" + opts.qmf_class + ":" + opts.id;
  query["_object_id"] = obj_id;
  query["_arguments"] = arguments;
  query["_method_name"] = args[0];

  std::cout << this->formatter.formatMap(this->agent.callQmfMethod(query)) << std::endl;

  this->agent.close();
  return 0;
}

int main(int argc, char ** argv)
{
  Options options;

  if (!options.parse(argc, argv) || options.getArguments().empty()) {
    options.error("wrong number of arguments");
    return(2);
  }

  QMF2QpidCtrl qmfc(options);

  int ecode = qmfc.main();
  return(ecode);
}

