#include "ConnectorHandler.h"

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;
using namespace dtests::common::log;

ConnectorHandler::ConnectorHandler(
    const string &url,
    vector<string> failover_urls,
    bool is_topic,
    string user,
    string password,
    string sasl_mechanisms,
    int timeout,
    string conn_reconnect,
    int32_t conn_reconnect_interval,
    int32_t conn_reconnect_limit,
    int32_t conn_reconnect_timeout,
    uint32_t conn_reconnect_first,
    uint32_t conn_reconnect_increment,
    bool conn_reconnect_doubling,
    bool conn_reconnect_custom,
    uint32_t max_frame_size
)
    : super(
        url,
        failover_urls,
        is_topic,
        user,
        password,
        sasl_mechanisms,
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

        exit(EXIT_SUCCESS);
    } else {
        timer--;
        logger(debug) << "Waiting ...";

        duration d = duration(1 * duration::SECOND.milliseconds());
        work_q->schedule(d, make_work(&ConnectorHandler::timerEvent, this));
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
    
    logger(debug) << "Maximum frame size: " << max_frame_size;

    connection_options conn_opts;
    
    if (!user.empty()) conn_opts.user(user);
    if (!password.empty()) conn_opts.password(password);

    conn_opts.sasl_enabled(true);
    conn_opts.sasl_allow_insecure_mechs(true);
    conn_opts.sasl_allowed_mechs(sasl_mechanisms);
    conn_opts.max_frame_size(max_frame_size);

    logger(debug) << "Setting a reconnect timer: " << conn_reconnect;
    logger(debug) << "Custom reconnect: " << conn_reconnect_custom;
    
    configure_reconnect(conn_opts);
    
    conn = c.connect(broker_url.getUri(), conn_opts);

    work_q = &conn.work_queue();
    
    if ((objectControl & SESSION)) {
        logger(trace) << "Creating the session as requested";
        sessionObj = conn.default_session();
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
    work_q->schedule(duration(0), make_work(&ConnectorHandler::timerEvent, this));
#endif
}

void ConnectorHandler::on_connection_open(connection &conn)
{
    logger(debug) << "Connected to " << broker_url.getHost() << ":" << broker_url.getPort();
    
    if ((objectControl & SESSION)) {
        logger(trace) << "Opening the session as requested";
        sessionObj.open();
    }
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
