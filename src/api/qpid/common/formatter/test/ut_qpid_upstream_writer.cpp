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
#include "formatter/UpstreamFormatter.h"
#include "formatter/AbstractDecoder.h"
#include "formatter/Writer.h"
#include "formatter/UpstreamWriter.h"
#include "formatter/QpidDecoder.h"

#include "TestUtils.h"
#include "formatter/UpstreamWriter.h"

using dtests::qpid::messaging::QpidDecoder;

using namespace std;
using namespace qpid::messaging;
using namespace qpid::types;
using namespace dtests::common;

static const char *expected = "Message(properties={'key1': 'value1'}, "
	"content='Test content')";

static Message buildMessage(const char *content) {
	Message message = Message();
	
	message.setContentType(ContentType::TEXT_PLAIN);
	message.setContent(content);
	
	message.setPriority(4);
	message.setReplyTo(Address("reply.to.queue"));
	
	message.setProperty("key1", "value1");

	return message;
}

/*
 * 
 */
int main(int argc, char** argv) {
	UpstreamFormatter formatter = UpstreamFormatter();
    
	std::ostringstream stream;
	UpstreamWriter writer = UpstreamWriter(&stream);
	
	Message message = buildMessage("Test content");
	
	QpidDecoder decoder = QpidDecoder(message); 
                                        
	formatter.printMessage(&decoder, &writer);
                    
	string actual = writer.toString();
	
	bool ret = assertEquals(expected, actual.c_str(), strlen(expected));
		
	if (!ret) {
		return 1;
	}

	return 0;
}

