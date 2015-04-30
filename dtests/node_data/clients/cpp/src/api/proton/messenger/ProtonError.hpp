/*
 * ProtonError.hpp
 *
 *  Created on: Apr 22, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_PROTONERROR_HPP_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_PROTONERROR_HPP_

#include <proton/error.h>

#include <boost/log/trivial.hpp>

#include <string>

using namespace std;

namespace dtests {
namespace proton {
namespace messenger {

class ProtonError {
public:
	template <typename T>
	static T handleError(pn_error_t *error, const string &message);
};


template <typename T>
T ProtonError::handleError(pn_error_t *error, const string &message) {
	BOOST_LOG_TRIVIAL(trace) << "An error occurred within Proton and the code "
				<< " will try to determine the root cause";

	const char *protonErrorText = pn_error_text(error);

	if (protonErrorText == NULL) {
		BOOST_LOG_TRIVIAL(warning) << "Unable to obtain Proton error text";

		return T(message + ": unspecified Proton internal error");
	}

	return T(message + ": " + protonErrorText);
}

}}}

#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_APIS_PROTON_MESSENGER_PROTONERROR_HPP_ */
