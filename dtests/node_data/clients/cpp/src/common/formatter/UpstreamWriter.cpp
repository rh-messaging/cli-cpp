/* 
 * File:   UpstreamWriter.cpp
 * Author: opiske
 * 
 * Created on May 11, 2015, 5:39 PM
 */

#include "UpstreamWriter.h"
#include "Writer.h"

const char *UpstreamWriter::START = "Message(";

UpstreamWriter::UpstreamWriter(ostringstream *stream)
    : super(),
    stream(stream)
{

}

UpstreamWriter::~UpstreamWriter()
{
    stream->flush();
}

void UpstreamWriter::start()
{
    (*stream) << START;
}

void UpstreamWriter::end()
{
    (*stream) << END;
}

void UpstreamWriter::startProperties()
{
    writePlain("properties=");
}

void UpstreamWriter::startContent()
{
    (*stream) << FIELD_SEPARATOR << ' ';
    writePlain("content=");
}

void UpstreamWriter::startMap()
{
    (*stream) << SECTION_START;
}

void UpstreamWriter::endMap()
{
    (*stream) << SECTION_END;
}

void UpstreamWriter::startList()
{
    (*stream) << SECTION_START;
}

void UpstreamWriter::endList()
{
    (*stream) << SECTION_END;
}

void UpstreamWriter::endField()
{
    (*stream) << FIELD_SEPARATOR << ' ';
}

void UpstreamWriter::writePlain(const string &str)
{
    (*stream) << str;

}

void UpstreamWriter::write(const string &str)
{
    (*stream) << VALUE_START << str << VALUE_END;

}

void UpstreamWriter::write(const KeyValue &keyValue)
{
    write(keyValue, true);
}

void UpstreamWriter::write(const KeyValue &keyValue, bool separator)
{
    write(keyValue.first);
    (*stream) << VALUE_SEPARATOR << ' ';
    write(keyValue.second);

    if (separator) {
        endField();
    }
}

void UpstreamWriter::writeBogus(const string &str)
{
    (*stream) << Writer::BOGUS_FORMAT_MESSAGE << VALUE_SEPARATOR << str;
}

void UpstreamWriter::endLine()
{
    (*stream) << std::endl;
}

string UpstreamWriter::toString() const
{
    stream->flush();
    return stream->str();
}

