/*
 * CommonHandler.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: opiske
 */

#include "CommonHandler.h"

namespace dtests {
namespace proton {
namespace reactor {

using namespace dtests::common;
using namespace dtests::common::log;

Logger CommonHandler::logger = LoggerWrapper::getLogger();


CommonHandler::CommonHandler(
    const string &url_str,
    vector<string> conn_urls,
    bool is_topic,
    string user,
    string password,
    string sasl_mechanisms,
    string conn_sasl_enabled,
    string conn_ssl_certificate,
    string conn_ssl_private_key,
    string conn_ssl_password,
    string conn_ssl_trust_store,
    bool conn_ssl_verify_peer,
    bool conn_ssl_verify_peer_name,
    int timeout,
    string conn_reconnect,
    int32_t conn_reconnect_interval,
    int32_t conn_reconnect_limit,
    int32_t conn_reconnect_timeout,
    uint32_t conn_reconnect_first,
    uint32_t conn_reconnect_increment,
    bool conn_reconnect_doubling,
    bool conn_reconnect_custom,
    uint32_t conn_heartbeat,
    uint32_t max_frame_size,
    bool conn_use_config_file,
    string log_msgs
)
    : super(),
    work_q(0),
    broker_url(url_str),
    conn_urls(conn_urls),
    is_topic(is_topic),
    user(user),
    password(password),
    sasl_mechanisms(sasl_mechanisms),
    conn_sasl_enabled(conn_sasl_enabled),
    conn_ssl_certificate(conn_ssl_certificate),
    conn_ssl_private_key(conn_ssl_private_key),
    conn_ssl_password(conn_ssl_password),
    conn_ssl_trust_store(conn_ssl_trust_store),
    conn_ssl_verify_peer(conn_ssl_verify_peer),
    conn_ssl_verify_peer_name(conn_ssl_verify_peer_name),
    conn_reconnect(conn_reconnect),
    conn_reconnect_interval(conn_reconnect_interval),
    conn_reconnect_limit(conn_reconnect_limit),
    conn_reconnect_timeout(conn_reconnect_timeout),
    conn_reconnect_first(conn_reconnect_first),
    conn_reconnect_increment(conn_reconnect_increment),
    conn_reconnect_doubling(conn_reconnect_doubling),
    conn_reconnect_custom(conn_reconnect_custom),
    conn_heartbeat(conn_heartbeat),
    max_frame_size(max_frame_size),
    conn_use_config_file(conn_use_config_file),
    log_msgs(log_msgs),
    cont(NULL),
    timer(timeout, "timeout"),
    timeout(timeout)
{
    logger(debug) << "Creating a handler for " << url_str;
}

CommonHandler::~CommonHandler()
{
    logger(debug) << "Destroying the common handler";
}

void CommonHandler::configure_reconnect(::proton::connection_options & conn_opts) {
    ::proton::reconnect_options ro;

    if (conn_reconnect == "true" && conn_reconnect_custom == false) {
        conn_opts.reconnect(ro);
    } else if (conn_reconnect == "true" && conn_reconnect_custom == true) {
        logger(debug) << "Reconnect first: " << conn_reconnect_first;
        logger(debug) << "Reconnect interval (max): " << conn_reconnect_interval;
        logger(debug) << "Reconnect increment: " << conn_reconnect_increment;
        logger(debug) << "Reconnect doubling: " << conn_reconnect_doubling;
        logger(debug) << "Reconnect limit (max_retries): " << conn_reconnect_limit;
        logger(debug) << "Reconnect timeout: " << conn_reconnect_timeout;

        ro.delay(duration(conn_reconnect_interval));
        if (conn_reconnect_doubling) {
            ro.delay_multiplier(2.0);
        }
        ro.max_attempts(conn_reconnect_limit);
        ro.max_delay(duration(conn_reconnect_timeout));
    }
}

void CommonHandler::configure_ssl(::proton::container & c) {
    logger(debug) << "Configuring SSL...";

    if (conn_ssl_trust_store != "" || conn_ssl_verify_peer_name || conn_ssl_verify_peer) {
        // Setup SSL server auth
        /* rkubis 2019-09-30 We have System CA as default trust store now
        if (conn_ssl_trust_store == "") {
            logger(error) << "Trust store must be given: '" << conn_ssl_trust_store << "'";
        }
        */

        ::proton::ssl::verify_mode client_mode = ::proton::ssl::ANONYMOUS_PEER;
        if (conn_ssl_verify_peer_name) {
            client_mode = ::proton::ssl::VERIFY_PEER_NAME;
        } else if (conn_ssl_verify_peer) {
            client_mode = ::proton::ssl::VERIFY_PEER;
        }

        ssl_client_options client_ssl(client_mode);
        connection_options client_opts;
        client_opts.ssl_client_options(client_ssl);
        c.client_connection_options(client_opts);
    }

    if (conn_ssl_certificate != "") {
        // Setup SSL client auth
        if (conn_ssl_private_key == "") {
            logger(error) << "Client private key must be given: '" << conn_ssl_private_key << "'";
        }

        ::proton::ssl::verify_mode server_mode = ::proton::ssl::ANONYMOUS_PEER;
        if (conn_ssl_verify_peer_name) {
            server_mode = ::proton::ssl::VERIFY_PEER_NAME;
        } else if (conn_ssl_verify_peer) {
            server_mode = ::proton::ssl::VERIFY_PEER;
        }

        connection_options server_opts;
        ssl_certificate client_cert = ssl_certificate(conn_ssl_certificate, conn_ssl_private_key, conn_ssl_password);
        ssl_server_options server_ssl(client_cert, conn_ssl_trust_store, string(), server_mode);
        server_opts.ssl_server_options(server_ssl);

        c.server_connection_options(server_opts);
    }
}

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */
