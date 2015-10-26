/* 
 * File:   DictFormatter.cpp
 * Author: opiske
 * 
 * Created on May 11, 2015, 5:55 PM
 */

#include "DictFormatter.h"

DictFormatter::DictFormatter()
    : super()

{
}

DictFormatter::DictFormatter(const DictFormatter& orig)
    : super()
{
}

DictFormatter::~DictFormatter()
{
}

void DictFormatter::printMessage(const AbstractDecoder *decoder, Writer *writer) const
{
    writer->startHeader();
    decoder->decodeHeader(writer);
    decoder->decodeProperties(writer);

    writer->startContent();
    decoder->decodeContent(writer);
    writer->endContent();
}


