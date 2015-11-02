/* 
 * File:   StringWriter.cpp
 * Author: opiske
 * 
 * Created on May 6, 2015, 4:13 PM
 */

#include "DictWriter.h"
#include "Writer.h"

using namespace dtests::common;

const char *DictWriter::STATISTICS_START = "STATS ";

DictWriter::DictWriter(ostringstream *stream)
    : super(),
    stream(stream)
{
}

DictWriter::~DictWriter()
{
    stream->flush();
}

void DictWriter::startHeader()
{
    (*stream) << HEADER_START;
}

void DictWriter::endHeader()
{
    (*stream) << HEADER_END;
}

void DictWriter::startProperties()
{
    (*stream) << ' ';
    write("properties");
    (*stream) << VALUE_SEPARATOR << ' ';
}

void DictWriter::endProperties()
{

}

void DictWriter::startContent()
{
    (*stream) << FIELD_SEPARATOR << ' ';
    write("content");
    (*stream) << VALUE_SEPARATOR << ' ';
}

void DictWriter::endContent()
{
    (*stream) << CONTENT_END;
}

void DictWriter::startMap()
{
    (*stream) << SECTION_START;
}

void DictWriter::endMap()
{
    (*stream) << SECTION_END;
}

void DictWriter::startList()
{
    (*stream) << SECTION_START;
}

void DictWriter::endList()
{
    (*stream) << SECTION_END;
}

void DictWriter::startStatistics()
{
    (*stream) << STATISTICS_START;
}

void DictWriter::endStatistics()
{
    (*stream) << SECTION_END;
}

void DictWriter::endField()
{
    (*stream) << FIELD_SEPARATOR << ' ';
}

void DictWriter::write(const string &str)
{
    (*stream) << VALUE_START << str << VALUE_END;

}

void DictWriter::write(const KeyValue &keyValue)
{
    write(keyValue, true);
}

void DictWriter::write(const KeyValue &keyValue, bool separator)
{
    write(keyValue.first);
    (*stream) << VALUE_SEPARATOR << ' ';
    write(keyValue.second);

    if (separator) {
        endField();
    }
}

void DictWriter::writeBogus(const string &str)
{
    (*stream) << Writer::BOGUS_FORMAT_MESSAGE << VALUE_SEPARATOR << str;
}

void DictWriter::endLine()
{
    (*stream) << std::endl;
}

string DictWriter::toString() const
{
    stream->flush();
    return stream->str();
}

