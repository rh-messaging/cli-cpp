#include "FormatUtil.h"

namespace dtests {
namespace qpid {
namespace messaging {
	
using std::cout;

void printMessageUpstream(const Message &message, bool verbose) {
#ifdef ENABLE_MODERN
	QpidDecoder decoder = QpidDecoder(message); 
		
	std::ostringstream stream;
	UpstreamWriter writer = UpstreamWriter(&stream);
	
	UpstreamFormatter formatter = UpstreamFormatter();
	formatter.printMessage(&decoder, &writer);
	
	writer.endLine();
	std::cout << writer.toString();
	 
#else
	Formatter formatter;
	
	Message tmp = const_cast<Message &>(message);
	
	formatter.printMessage(tmp, verbose);
#endif // ENABLE_MODERN
}


void printMessageDict(const Message &message) {
#ifdef ENABLE_MODERN
	QpidDecoder decoder = QpidDecoder(message); 
	
	std::ostringstream stream;
	DictWriter writer = DictWriter(&stream);
	
	DictFormatter formatter = DictFormatter();
	formatter.printMessage(&decoder, &writer);
	
	writer.endLine();
	std::cout << writer.toString();
#else
	Formatter formatter;
	
	Message tmp = const_cast<Message &>(message);
	
	formatter.printMessageAsDict(tmp);
#endif // ENABLE_MODERN
}


void printStatistics(const Message &message, const Variant::Map &stats) {
#ifdef ENABLE_MODERN
	/* 
	 * NOTE: this is *not* the best way to solve this problem and I will 
	 * implement a more elegant design in the future
	 */
	QpidDecoder decoder = QpidDecoder(message); 
	
	std::ostringstream stream;
	DictWriter writer = DictWriter(&stream);
	
	DictFormatter formatter = DictFormatter();
	formatter.printStatistics(&decoder, &writer, stats);
#else
	Formatter formatter;
	
	Variant::Map tmp = const_cast<Variant::Map &>(stats);
	
	formatter.printStatistics(tmp);
#endif // ENABLE_MODERN
}
	
}}}
