#include "ConnectorHandler.h"

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;
using namespace dtests::common::log;

ConnectorHandler::ConnectorHandler(
    const string &url,
    vector<string> conn_urls,
    bool is_topic,
    string user,
    string password,
    string sasl_mechanisms,
    string conn_sasl_enabled,
    int timeout,
    string conn_reconnect,
    int32_t conn_reconnect_interval,
    int32_t conn_reconnect_limit,
    int32_t conn_reconnect_timeout,
    uint32_t conn_reconnect_first,
    uint32_t conn_reconnect_increment,
    bool conn_reconnect_doubling,
    bool conn_reconnect_custom,
    uint32_t max_frame_size,
    bool use_default_connection
)
    : super(
        url,
        conn_urls,
        is_topic,
        user,
        password,
        sasl_mechanisms,
        conn_sasl_enabled,
        timeout,
        conn_reconnect,
        conn_reconnect_interval,
        conn_reconnect_limit,
        conn_reconnect_timeout,
        conn_reconnect_first,
        conn_reconnect_increment,
        conn_reconnect_doubling,
        conn_reconnect_custom,
        max_frame_size
    ),
    objectControl(CONNECTION),
    timer_event(*this),
    use_default_connection(use_default_connection)
{
    logger(debug) << "Initializing the connector handler";

    logger(debug) << "Using default connection: " << use_default_connection;
}

void ConnectorHandler::timerEvent() {
#if defined(__REACTOR_HAS_TIMER)
    if (timer.isExpired()) {
        logger(info) << "Timed out";

        closeObjects();
    } else {
        timer--;
        logger(debug) << "Waiting ...";

        work_q->schedule(duration::SECOND, make_work(&ConnectorHandler::timerEvent, this));
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

    if (use_default_connection) {
        connector_conn = c.connect();
    } else {
        logger(debug) << "User: " << user;
        logger(debug) << "Password: " << password;
        logger(debug) << "SASL mechanisms: " << sasl_mechanisms;
        logger(debug) << "SASL enabled: " << conn_sasl_enabled;
        
        logger(debug) << "Maximum frame size: " << max_frame_size;

        connection_options conn_opts;
        
        if (!user.empty()) conn_opts.user(user);
        if (!password.empty()) conn_opts.password(password);

        if (conn_sasl_enabled == "false") {
            conn_opts.sasl_enabled(false);
        } else {
            conn_opts.sasl_enabled(true);
        }

        conn_opts.sasl_allow_insecure_mechs(true);
        conn_opts.sasl_allowed_mechs(sasl_mechanisms);
        conn_opts.max_frame_size(max_frame_size);

        logger(debug) << "Setting a reconnect timer: " << conn_reconnect;
        logger(debug) << "Custom reconnect: " << conn_reconnect_custom;
        
        configure_reconnect(conn_opts);
        
        connector_conn = c.connect(broker_url.getUri(), conn_opts);
    }
}

void ConnectorHandler::on_connection_open(connection &conn)
{
    logger(debug) << "Connected to " << broker_url.getHost() << ":" << broker_url.getPort();

    work_q = &conn.work_queue();
    
    if ((objectControl & SESSION)) {
        logger(trace) << "Creating the session as requested";
        sessionObj = conn.default_session();
        logger(trace) << "Opening the session as requested";
        sessionObj.open();
    }

    if ((objectControl & SENDER)) {
        logger(trace) << "Opening the sender as requested";
        senderObj = conn.open_sender(broker_url.getPath());
    }

    if ((objectControl & RECEIVER)) {
        logger(trace) << "Opening the receiver as requested";
        receiverObj = conn.open_receiver(broker_url.getPath());
    }
    
#if defined(__REACTOR_HAS_TIMER)
    work_q->schedule(duration::IMMEDIATE, make_work(&ConnectorHandler::timerEvent, this));
#endif
}


void ConnectorHandler::on_connection_close(connection &conn)
{
    logger(debug) << "Closing the connection to " << broker_url.getHost() << ":" << broker_url.getPort();
}

void ConnectorHandler::on_connection_error(connection &conn)
{
    logger(error) << "Failed to connect to " << broker_url.getHost() << ":" << broker_url.getPort();

    if (conn.error().what().find("Unable to validate user") != string::npos) {
        exit(1);
    }
}

void ConnectorHandler::on_transport_error(transport &trans) {
    logger(error) << "The connection with " << broker_url.getHost() << ":" << broker_url.getPort() << " was interrupted: " << trans.error().what();
}

void ConnectorHandler::on_transport_close(transport &t) {
    logger(debug) << "Closing the transport";

    if (conn_reconnect == "false") {
        exit(1);
    }
}

void ConnectorHandler::on_session_error(session &s) {
    logger(error) << "The remote peer at " << broker_url.getHost() << ":" << broker_url.getPort() <<
            " closed the session with an error condition";
    closeObjects();
} 


void ConnectorHandler::setObjectControl(int control) {
    objectControl = control;
}

void ConnectorHandler::closeObjects() {
    connection conn = connector_conn;

    if ((objectControl & RECEIVER)) {
        logger(trace) << "Closing the receiver";
        if (!receiverObj.uninitialized()) {
            conn = receiverObj.connection();
            receiverObj.close();
            logger(trace) << "Receiver closed";
        }
    }

    if ((objectControl & SENDER)) {
        logger(trace) << "Closing the sender";
        if (!senderObj.uninitialized()) {
            conn = senderObj.connection();
            senderObj.close();
            logger(trace) << "Sender closed";
        }
    }
    
    if ((objectControl & SESSION)) {
        logger(trace) << "Closing the session (currently ignored)";
        if (!sessionObj.uninitialized()) {
            conn = sessionObj.connection();
            sessionObj.close();
            logger(trace) << "Session closed";
        }
    }
    
    if (!conn.uninitialized()) {
        conn.close();
        logger(trace) << "Connection closed";
    } else {
        connector_conn.close();
        logger(trace) << "Connector connection closed";
    }
}


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
