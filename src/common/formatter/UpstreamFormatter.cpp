/* 
 * File:   DictFormatter.cpp
 * Author: opiske
 * 
 * Created on May 11, 2015, 5:55 PM
 */

#include "UpstreamFormatter.h"

UpstreamFormatter::UpstreamFormatter()
	: super() 

{ }

UpstreamFormatter::UpstreamFormatter(const UpstreamFormatter& orig)
	: super()
{ }

UpstreamFormatter::~UpstreamFormatter() { }


void UpstreamFormatter::printMessage(const AbstractDecoder *decoder, Writer *writer) const {
	writer->start();
	decoder->decodeProperties(writer);
	
	writer->startContent();
	decoder->decodeContent(writer);
	writer->endContent();
	
	writer->end();
}


