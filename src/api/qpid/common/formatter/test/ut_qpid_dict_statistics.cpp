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
using namespace dtests::common;

static const char *expected = "STATS {'available': '3', 'capacity': '5', "
	"'sender': 'None', 'timestamp': '0', 'unsettled': '1'}";


static map<string, string> getStatistics() {
	map<string, string> ret = map<string, string>(); 
	
	ret["capacity"] = "5"; 
	ret["available"] = "3";
	ret["unsettled"] = "1"; 
	ret["sender"] = "None"; 
	ret["timestamp"] = "0";
	
	return ret;
}

/*
 * 
 */
int main(int argc, char** argv) {
	std::ostringstream stream;
	DictWriter writer = DictWriter(&stream);
	
	Message message = Message();
	map<string, string> stats = getStatistics();
	
	QpidDecoder decoder = QpidDecoder(message); 
        
	DictFormatter formatter = DictFormatter();
	formatter.printStatistics(&decoder, &writer, stats);
	
	string actual = writer.toString();
	bool ret = assertEquals(expected, actual.c_str(), strlen(expected));
		
	if (!ret) {
		return 1;
	}

	return 0;
}

