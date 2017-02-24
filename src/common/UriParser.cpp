/*
 * UriParser.cpp
 *
 *  Created on: Feb 23, 2017
 *      Author: rkubis
 */

#include "UriParser.h"

using namespace dtests::common;

UriParser::UriParser() :
    uri(""),
    user(""),
    password(""),
    host(""),
    port(""),
    path("")
{
    // TODO Auto-generated constructor stub

}

UriParser::UriParser(const string uri)
{
    UriParser();

    this->parse(uri);
}

UriParser::~UriParser()
{
    // TODO Auto-generated destructor stub
}

void UriParser::parse(const string uri)
{    
    this->uri = uri;

    string::size_type colon_position = this->uri.find(":");
    string::size_type ampersand_position = this->uri.find("@");
    string::size_type slash_position = this->uri.find("/");

    if (ampersand_position != string::npos && colon_position != string::npos && colon_position < ampersand_position) {
        this->user = this->uri.substr(0, colon_position);
        this->password = this->uri.substr((colon_position + 1), (ampersand_position - colon_position - 1));

        colon_position = this->uri.find(":", ampersand_position);
    }

    if (colon_position != string::npos && slash_position != string::npos) {
        this->host = this->uri.substr((ampersand_position + 1), (colon_position - ampersand_position - 1));
        this->port = this->uri.substr((colon_position + 1), (slash_position - colon_position - 1));
    } else if (colon_position == string::npos && slash_position != string::npos) {
        this->host = this->uri.substr((ampersand_position + 1), (slash_position - ampersand_position - 1));
    } else if (colon_position != string::npos && slash_position == string::npos) {
        this->host = this->uri.substr((ampersand_position + 1), (colon_position - ampersand_position - 1));
        this->port = this->uri.substr(colon_position - ampersand_position - 1);
    } else {
        this->host = this->uri.substr(ampersand_position + 1);
    }

    if (slash_position != string::npos) {
        this->path = this->uri.substr(slash_position + 1);
    }
}

string UriParser::getUri() {
    return this->uri;
}

string UriParser::getUser() {
    return this->user;
}

string UriParser::getPassword() {
    return this->password;
}

string UriParser::getHost() {
    return this->host;
}

string UriParser::getPort() {
    return this->port;
}

string UriParser::getPath() {
    return this->path;
}

