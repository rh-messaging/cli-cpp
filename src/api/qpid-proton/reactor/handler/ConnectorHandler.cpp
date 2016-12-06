#include "ConnectorHandler.h"

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;
using namespace dtests::common::log;

ConnectorHandler::ConnectorHandler(const string &url, string user, string password, string sasl_mechanisms, int timeout, string conn_reconnect)
    : super(url, user, password, sasl_mechanisms, timeout, conn_reconnect),
      objectControl(CONNECTION),
        timer_event(*this)
{
    logger(debug) << "Initializing the connector handler";

}

ConnectorHandler::~ConnectorHandler()
{
    closeObjects();
}

void ConnectorHandler::timerEvent() {
#if defined(__REACTOR_HAS_TIMER)
    if (timer.isExpired()) {
        logger(info) << "Timed out";

        senderObj.container().stop();
    } else {
        timer--;
        logger(debug) << "Waiting ...";

    }
#endif
}

void ConnectorHandler::setCount(int count)
{
    this->count = count;
}

int ConnectorHandler::getCount() const
{
    return count;
}

void ConnectorHandler::on_container_start(container &c)
{
    logger(debug) << "Starting messaging handler";

    logger(trace) << "Creating connection";

    logger(debug) << "User: " << user;
    logger(debug) << "Password: " << password;
    logger(debug) << "SASL mechanisms: " << sasl_mechanisms;
    
    logger(debug) << "Setting a reconnect timer: " << conn_reconnect;

    connection_options conn_opts = c.client_connection_options()
                                    .user(user)
                                    .password(password)
                                    .sasl_enabled(true)
                                    .sasl_allow_insecure_mechs(true)
                                    .sasl_allowed_mechs(sasl_mechanisms);

    if (conn_reconnect == "default") {
        conn_opts = conn_opts.reconnect(reconnect_timer());
    }

    conn = c.connect(broker_url, conn_opts);
    
    if ((objectControl & SESSION)) {
        logger(trace) << "Creating the session as requested";
        sessionObj = conn.default_session();
    }
    
    if ((objectControl & SENDER)) {
        logger(trace) << "Opening the sender as requested";
        senderObj = conn.open_sender(broker_url.host_port());
    }
    
    if ((objectControl & RECEIVER)) {
        logger(trace) << "Opening the receiver as requested";
        receiverObj = conn.open_receiver(broker_url.host_port());
    }
    
    duration d = duration(int(1000 * duration::SECOND.milliseconds()));
#if defined(__REACTOR_HAS_TIMER)
    c.schedule(d, timer_event);
#endif
}

void ConnectorHandler::on_connection_open(connection &conn)
{
    logger(debug) << "Connected to " << broker_url.host_port();
    
    if ((objectControl & SESSION)) {
        logger(trace) << "Opening the session as requested";
        sessionObj.open();
    }
}


void ConnectorHandler::on_connection_close(connection &conn)
{
    logger(debug) << "Closing the connection to " << broker_url.host_port();
}

void ConnectorHandler::on_connection_error(connection &conn)
{
    logger(error) << "Failed to connect to " << broker_url.host_port();
}

void ConnectorHandler::on_transport_error(transport &trans) {
    logger(error) << "The connection with " << broker_url.host_port() << 
            " was interrupted";
}

void ConnectorHandler::on_transport_close(transport &t) {
    logger(debug) << "Closing the transport";
}


void ConnectorHandler::on_session_error(session &s) {
    logger(error) << "The remote peer at " << broker_url.host_port() << 
            " closed the session with an error condition";
    closeObjects();
} 


void ConnectorHandler::setObjectControl(int control) {
    objectControl = control;
}

void ConnectorHandler::closeObjects() {
    if ((objectControl & RECEIVER)) {
        logger(trace) << "Closing the receiver";
        receiverObj.close();
    }
    
    if ((objectControl & SENDER)) {
        logger(trace) << "Closing the sender";
        senderObj.close();
    }
    
    if ((objectControl & SESSION)) {
        logger(trace) << "Closing the session (currently ignored)";
        sessionObj.close();
    }
    
    conn.close();
}


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
