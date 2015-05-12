/* 
 * File:   ut_qpid_decoder.cpp
 * Author: opiske
 *
 * Created on May 7, 2015, 2:42 PM
 */

#include <cstdlib>

#include <qpid/messaging/Message.h>
#include <qpid/messaging/Address.h>

#include "formatter/DictFormatter.h"
#include "formatter/Writer.h"
#include "formatter/DictWriter.h"
#include "formatter/QpidDecoder.h"

#include "TestUtils.h"

using dtests::qpid::messaging::QpidDecoder;

using namespace std;
using namespace qpid::messaging;
using namespace qpid::types;

/*
static const char *expected = "{'redelivered': 'False', 'reply_to': "
	"'reply.to.queue', 'subject': 'None', 'content_type': 'text/plain', "
	"'user_id': 'None', 'id': 'None', 'correlation_id': 'None', "
	"'priority': '4', 'durable': 'False', 'ttl': '0', 'size': '12',  "
	"'properties': {'key1': 'value1'}, 'content': 'Test content'}";
 */

bool testKeyValue() {
	// Note: the size is none because the content type is not text/plain
	const char *expected = "'redelivered': 'False', 'reply_to': "
	"'None', 'subject': 'None', 'content_type': 'None', "
	"'user_id': 'None', 'id': 'None', 'correlation_id': 'None', "
	"'priority': '0', 'durable': 'False', 'ttl': '0', 'size': 'None'";
	
	
	std::ostringstream stream;
	DictWriter writer = DictWriter(&stream);
	Message message = Message();

	QpidDecoder decoder = QpidDecoder(message);
	decoder.decodeHeader(&writer);

	string decodedStr = stream.str();
	stream.clear();
	
	return assertEquals(expected, decodedStr.c_str());
}


bool testMap() {
	const char *expected = " 'properties': {'key1': 'value1'}";
	
	std::ostringstream stream;
	DictWriter writer = DictWriter(&stream);
	Message message = Message();
	
	Variant::Map properties = Variant::Map();
	properties["key1"] = "value1";
	message.setProperties(properties);

	QpidDecoder decoder = QpidDecoder(message);
	decoder.decodeProperties(&writer);
	
	string decodedStr = stream.str();
	stream.clear();
	
	return assertEquals(expected, decodedStr.c_str());
}

/*
 * 
 */
int main(int argc, char** argv) {
	if (!testKeyValue()) {
		return EXIT_FAILURE;
	}
	
	if (!testMap()) {
		return EXIT_FAILURE;
	}

}

