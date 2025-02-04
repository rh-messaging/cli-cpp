/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TxReceiverHandler.h
 * Author: pematous
 *
 * Created on November 20, 2024
 */

#ifndef TXRECEIVERHANDLER_H
#define TXRECEIVERHANDLER_H

#include <proton/transaction.hpp>

#include "ReceiverHandler.h"

using proton::transaction;
using proton::session;
using proton::transaction_handler;

#ifdef PN_CPP_HAS_STD_FUNCTION
#undef PN_CPP_HAS_STD_FUNCTION
#endif

namespace dtests {
namespace proton {
namespace reactor {

/**
 * A proton transaction message handler that handles message receive events
 */
class TxReceiverHandler : public ReceiverHandler, transaction_handler {
  public:
    /**
     * Constructor
     * @param url broker URL
     * @param conn_urls connection URLs
     * @param is_topic if target is topic
     * @param durable_subscriber durable subscription to topic
     * @param subscriber_unsubscribe unsubscribe durable subscriptor
     * @param durable_subscriber_prefix prefix to use to identify subscriber
     * @param durable_subscriber_name name of the durable subscriber to be unsubscribe
     * @param shared_subscriber shared subscription to topic
     * @param msg_action message action
     * @param msg_action_size apply action in the batch of given size
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
     * @param count count of messages to receive
     * @param duration_time message actions total duration
     * @param duration_mode specifies where to wait to achieve expected duration
     * @param conn_reconnect type of reconnection
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
     * @param process_reply_to send message to reply-to address if enabled and message got reply-to address
     * @param browse enable browsing receiver
     * @param recv_listen enable p2p listener
     * @param recv_listen_port p2p listener port
     * @param recv_credit_window receiver credit window
     * @param recv_drain_after_credit_window drain aqfter credit window
     * @param tx_action transaction action on batch
     * @param tx_endloop_action transaction action on last batch
     */
    TxReceiverHandler(
        const string &url,
        vector<string> conn_urls,
        bool is_topic,
        bool durable_subscriber,
        bool subscriber_unsubscribe,
        string durable_subscriber_prefix,
        string durable_subscriber_name,
        bool shared_subscriber,
        string msg_action,
        int msg_action_size,
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
        int count = 0,
        int duration_time = 0,
        string duration_mode = "after-receive",
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
        bool process_reply_to = false,
        bool browse = false,
        string recv_listen = "false",
        int recv_listen_port = 5672,
        int recv_credit_window = -1,
        bool recv_drain_after_credit_window = false,
        string tx_action = "commit",
        string tx_endloop_action = "commit"
    );

    virtual ~TxReceiverHandler();

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

    // reactor method
    void on_session_open(session &s);
    void on_transaction_declare_failed(transaction);
    void on_transaction_commit_failed(transaction t);
    void on_transaction_declared(transaction t);
    void on_transaction_committed(transaction t);
    void on_transaction_aborted(transaction t);

    // overrides
    void on_container_start(container &c);
    void on_message(delivery &d, message &m);
    void on_transport_close(transport &t);

  private:
    typedef ReceiverHandler super;

    int batch_size = 0;
    int current_batch = 0;
    int processed = 0;
    string tx_action = "commit";
    string tx_endloop_action = "commit";
    transaction tx;
    session sess;
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* TXRECEIVERHANDLER_H */

