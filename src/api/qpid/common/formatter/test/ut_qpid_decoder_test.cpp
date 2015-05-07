/* 
 * File:   ut_qpid_decoder.cpp
 * Author: opiske
 *
 * Created on May 7, 2015, 2:42 PM
 */

#include <cstdlib>

#include <qpid/messaging/Message.h>
#include <qpid/messaging/Address.h>

#include "formatter/AbstractFormatter.h"
#include "formatter/AbstractDecoder.h"
#include "formatter/Writer.h"
#include "formatter/StringWriter.h"
#include "formatter/QpidDecoder.h"

using dtests::qpid::messaging::QpidDecoder;

using namespace std;
using namespace qpid::messaging;
using namespace qpid::types;

static const char *expected = "{'redelivered': 'False', 'reply_to': "
	"'reply.to.queue', 'subject': 'None', 'content_type': 'text/plain', "
	"'user_id': 'None', 'id': 'None', 'correlation_id': 'None', "
	"'priority': '', 'durable': 'False', 'ttl': '0', 'size': '12',  "
	"'properties': {'key1''value1'}, 'content': 'Test Content'}";

static Message buildMessage(const char *content) {
	Message message = Message();
	
	message.setContentType(ContentType::TEXT_PLAIN);
	message.setContent(content);
	
	message.setReplyTo(Address("reply.to.queue"));
	
	Variant::Map properties = Variant::Map();
	properties["key1"] = "value1";
	
	message.setProperties(properties);

	return message;
}

/*
 * 
 */
int main(int argc, char** argv) {
	 AbstractFormatter formatter = AbstractFormatter();
    
	std::ostringstream stream;
	StringWriter writer = StringWriter(&stream);
	
	Message message = buildMessage("Test content");
	
	QpidDecoder decoder = QpidDecoder(message); 
                                        
	formatter.printMessage(&decoder, &writer);
                    
	string ret = writer.toString();
		
	if (ret != expected) {
		std::cerr << "The strings do not match:" << std::endl;
		std::cerr << "Expected: " << expected << std::endl;
		std::cerr << "Got: " << ret << std::endl;
		
		return 1;
	}

	
	return 0;
}

