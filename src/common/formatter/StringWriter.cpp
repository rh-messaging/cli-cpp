/* 
 * File:   StringWriter.cpp
 * Author: opiske
 * 
 * Created on May 6, 2015, 4:13 PM
 */

#include "StringWriter.h"
#include "Writer.h"

StringWriter::StringWriter(ostringstream *stream)
	: super(),
	stream(stream) { }

StringWriter::~StringWriter() {
	stream->flush();
}

void StringWriter::startHeader() {
	(*stream) << HEADER_START;
}

void StringWriter::endHeader() {
	(*stream) << HEADER_END;
}

void StringWriter::startProperties() {
	(*stream) << ' ';
	write("properties");
	(*stream) << VALUE_SEPARATOR << ' ';
}

void StringWriter::endProperties() {
	
}

void StringWriter::startContent() {
	(*stream) << FIELD_SEPARATOR << ' ';
	write("content");
	(*stream) << VALUE_SEPARATOR << ' ';
}

void StringWriter::endContent() {
	(*stream) << CONTENT_END;
}

void StringWriter::startMap() {
	(*stream) << SECTION_START;
}

void StringWriter::endMap() {
	(*stream) << SECTION_END;
}

void StringWriter::startList() {
	(*stream) << SECTION_START;
}

void StringWriter::endList() {
	(*stream) << SECTION_END;
}

void StringWriter::write(const string &str) {
	(*stream) << VALUE_START << str << VALUE_END;

}

void StringWriter::write(const KeyValue &keyValue) {
	write(keyValue.first);
	(*stream) << VALUE_SEPARATOR << ' ';
	write(keyValue.second);
	(*stream) << FIELD_SEPARATOR << ' ';
}

void StringWriter::writeBogus(const string &str) {
	(*stream) << Writer::BOGUS_FORMAT_MESSAGE << VALUE_SEPARATOR << str;
}

void StringWriter::endLine() {
	(*stream) << std::endl;
}

string StringWriter::toString() const {
	stream->flush();
	return stream->str();
}

