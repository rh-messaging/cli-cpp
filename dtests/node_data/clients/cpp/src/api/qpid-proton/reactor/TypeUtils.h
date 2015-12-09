#ifndef TYPEUTILS_H
#define TYPEUTILS_H

#include <proton/message.hpp>
#include <proton/types.hpp>

using proton::message;
using proton::message_id;
using proton::amqp_timestamp;
using proton::value;

namespace dtests {
namespace proton {
namespace reactor {

/**
 * A std::string setter type
 */
typedef void (message::*string_setter)(const string &);

/**
 * A proton::amqpsts setter type
 */
typedef void (message::*amqpts_setter)(amqp_timestamp);

/**
 * A proton::data setter type
 */
typedef void (message::*data_setter)(const data &);

/**
 * A proton::data setter type
 */
typedef void (message::*value_setter)(const value &);


/**
 * A proton::data setter type
 */
typedef void (message::*id_setter)(const message_id &);


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */

#endif /* TYPEUTILS_H */

