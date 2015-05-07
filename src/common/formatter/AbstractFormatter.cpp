/* 
 * File:   AbstractFormatter.cpp
 * Author: opiske
 * 
 * Created on May 5, 2015, 12:50 PM
 */

#include "AbstractFormatter.h"

AbstractFormatter::AbstractFormatter() { }

AbstractFormatter::AbstractFormatter(const AbstractFormatter& orig) { }

AbstractFormatter::~AbstractFormatter() { }

void AbstractFormatter::printMessage(const AbstractDecoder *decoder, Writer *writer) const {
	writer->startHeader();
	decoder->decodeHeader(writer);
	decoder->decodeProperties(writer);
	
	writer->startContent();
	decoder->decodeContent(writer);
	writer->endContent();
}




