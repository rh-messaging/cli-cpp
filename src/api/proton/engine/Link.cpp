/*
 * Link.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: opiske
 */

#include "Link.h"

using namespace std;

Link::Link()
	: link(NULL),
	  terminus(NULL)
{
	// TODO Auto-generated constructor stub

}

Link::Link(const Session &session)
	: link(NULL),
	  terminus(NULL)
{
	init(session);
}

Link::~Link() {
	pn_link_free(link);
	// pn_terminus_free(terminus);
}

void Link::init(const Session &session) {
	BOOST_LOG_TRIVIAL(debug) << "Initializing the link";

	link = pn_sender(session.getSession(), "Test Sender");
	if (link == NULL) {
		BOOST_LOG_TRIVIAL(error) << "Unable to initialize sender link";
	}

	terminus = pn_link_target(link);
	if (terminus == NULL) {
		BOOST_LOG_TRIVIAL(debug) << "Unable to access the link target";
	}

	BOOST_LOG_TRIVIAL(debug) << "Setting the target address for the endpoint";
	pn_terminus_set_address(terminus, "test");
}


void Link::open() {
	BOOST_LOG_TRIVIAL(debug) << "Opening the link";
	pn_link_open(link);
}

void Link::send() {
	string data = string("hello");
	BOOST_LOG_TRIVIAL(debug) << "Sending data through the link";

	int credit = pn_link_credit(link);
	BOOST_LOG_TRIVIAL(trace) << "Link credit: " << credit;

	pn_delivery_tag_t dtag = pn_dtag(data.c_str(), data.length());
	pn_delivery_t *delivery = pn_delivery(link, dtag);
	if (delivery == NULL) {
		BOOST_LOG_TRIVIAL(error) << "Error creating a delivery object";
	}

	unsigned int sent = data.length();
	ssize_t ret = pn_link_send(link, data.c_str(), sent);

	if (ret != sent) {
		BOOST_LOG_TRIVIAL(error) << "Error sending data: sent " <<
				ret << " got " << sent;

		pn_error_t *error = pn_link_error(link);

		const char *text = pn_error_text(error);
		BOOST_LOG_TRIVIAL(error)  << "Error: " << text;
	}
	else {
		BOOST_LOG_TRIVIAL(info) << "Successfully sent " << sent << " bytes";
	}

	pn_delivery_settle(delivery);
	pn_link_advance(link);

	int remaingCredit = pn_link_credit(link);
	BOOST_LOG_TRIVIAL(trace) << "Remaining credit: " << remaingCredit;
}

