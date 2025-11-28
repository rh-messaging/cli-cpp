/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TxSenderHandler.h
 * Author: pematous
 *
 * Created on November 20, 2024
 */

#ifndef TXSENDERHANDLER_H
#define TXSENDERHANDLER_H

#include "SenderHandler.h"

using proton::session;

namespace dtests {
namespace proton {
namespace reactor {

using dtests::common::Timer;

/**
 * A proton transaction message handler that handles message send events
 */
class TxSenderHandler : public SenderHandler {
  public:
    /**
     * Constructor
     * @param url broker URL
     * @param conn_urls connection URLs
     * @param is_topic if target is topic
     * @param user username
     * @param password password
     * @param sasl_mechanisms SASL mechanisms
     * @param conn_sasl_enabled enable connection SASL
     * @param conn_ssl_certificate path to client certificate
     * @param conn_ssl_private_key path to client private key
     * @param conn_ssl_password client's certificate database password
     * @param conn_ssl_trust_store path to client trust store
     * @param conn_ssl_verify_peer verifies server certificate
     * @param conn_ssl_verify_peer_name verifies connection url against server hostname
     * @param timeout timeout
     * @param duration message actions total duration
     * @param duration_mode specifies where to wait to achieve expected duration
     * @param conn_reconnect type or reconnection
     * @param conn_reconnect_interval reconnect interval
     * @param conn_reconnect_limit reconnect limit
     * @param conn_reconnect_timeout reconnect timeout
     * @param conn_reconnect_first reconnect first
     * @param conn_reconnect_increment reconnect increment
     * @param conn_reconnect_doubling reconnect doubling
     * @param conn_reconnect_custom custom reconnect values
     * @param conn_heartbeat connection heartbeat in seconds
     * @param max_frame_size maximum frame size
     * @param conn_use_config_file use configuration file for connection
     * @param log_msgs message log format
     * @param tx_action transaction action on batch
     * @param tx_endloop_action transaction action on last batch
     */
    TxSenderHandler(
        const string &url,
        vector<string> conn_urls,
        bool is_topic,
        string user,
        string password,
        string sasl_mechanisms,
        string conn_sasl_enabled = "true",
        string conn_ssl_certificate = "",
        string conn_ssl_private_key = "",
        string conn_ssl_password = "",
        string conn_ssl_trust_store = "",
        bool conn_ssl_verify_peer = false,
        bool conn_ssl_verify_peer_name = false,
        int timeout = 10,
        int duration_time = 0,
        string duration_mode = "after-send",
        string conn_reconnect = "true",
        int32_t conn_reconnect_interval = -1,
        int32_t conn_reconnect_limit = -1,
        int32_t conn_reconnect_timeout = -1,
        uint32_t conn_reconnect_first = 0,
        uint32_t conn_reconnect_increment = 100,
        bool conn_reconnect_doubling = true,
        bool conn_reconnect_custom = false,
        uint32_t conn_heartbeat = 0,
        uint32_t max_frame_size = -1,
        bool conn_use_config_file = false,
        string log_msgs = "",
        string tx_action = "commit",
        string tx_endloop_action = "commit"
    );

    virtual ~TxSenderHandler();

    /**
     * Sets the transaction batch size
     * @param batch_size the transaction batch size
     */
    void setBatchSize(int batchSize);

    /**
     * Gets the transaction batch size
     * @return the transaction batch size
     */
    int getBatchSize() const;

    // overrides
    void checkIfCanSend();
    void send();

    // reactor methods
    void on_sender_close(sender &s);
    void on_session_transaction_error(session &s);
    void on_session_transaction_committed(session &s);
    void on_session_transaction_aborted(session &s);
    void on_session_transaction_declared(session &s);

    // overrides
    void on_container_start(container &c);
    void on_session_open(session &s);
    void on_sendable(sender &s);
    void on_connection_close(connection &c);

  private:
    typedef SenderHandler super;

    int batch_size = 10;
    int current_batch = 0;
    int processed = 0;
    string tx_action = "commit";
    string tx_endloop_action = "none";
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* TXSENDERHANDLER_H */

