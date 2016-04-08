#include "ConnectorHandler.h"

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;
using namespace dtests::common::log;

ConnectorHandler::ConnectorHandler(const string &url)
    : super(url)
{
    logger(debug) << "Initializing the connector handler";

}

ConnectorHandler::~ConnectorHandler()
{
}

void ConnectorHandler::setCount(int count)
{
    this->count = count;
}

int ConnectorHandler::getCount() const
{
    return count;
}

void ConnectorHandler::on_start(event &e)
{
    logger(debug) << "Starting messaging handler";

    for (int i = 0; i < getCount(); i++) {
        logger(trace) << "Creating connection " << i << " of " << getCount();

        e.container().connect(broker_url);
    }

}

void ConnectorHandler::on_connection_opened(event& e)
{
    logger(debug) << "Connected to " << broker_url.host_port();

    connection conn = e.connection();

    logger(debug) << "Closing connection";
    conn.close();
    logger(debug) << "Connection closed";
}

void ConnectorHandler::on_connection_error(event &e)
{
    logger(error) << "Failed to connect to " << broker_url.host_port();
}

void ConnectorHandler::on_connection_local_open(event& e)
{
    logger(debug) << "Local connection opened to " << broker_url.host_port();
}

void ConnectorHandler::on_connection_remote_open(event& e)
{
    logger(debug) << "Remote connection opened to " << broker_url.host_port();
}


void ConnectorHandler::on_transport_error(event &e) {
    logger(error) << "The connection with " << broker_url.host_port() << 
            " was interrupted";
}


void ConnectorHandler::on_session_error(event &e) {
    logger(error) << "The remote peer at " << broker_url.host_port() << 
            " closed the session with an error condition";
} 


void ConnectorHandler::on_link_error(event &e) {
    logger(error) << "The remote peer at " << broker_url.host_port() << 
            " closed the link with an error condition";
}


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
