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
    scheme(""),
    user(""),
    password(""),
    host(""),
    port(""),
    path(""),
    query(""),
    fragment("")
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

    string::size_type scheme_delimiter = this->uri.find("://");
    this->scheme = (scheme_delimiter != string::npos) ? this->uri.substr(0, scheme_delimiter) : "";

    string::size_type colon_position = (scheme_delimiter != string::npos) ? this->uri.find(":", (scheme_delimiter + 1)) : this->uri.find(":");
    string::size_type ampersand_position = this->uri.find("@");
    string::size_type rightbracket_position = this->uri.find("]");
    string::size_type slash_position = (scheme_delimiter != string::npos) ? this->uri.find("/", (scheme_delimiter + 3)) : this->uri.find("/");
    string::size_type questionmark_position = this->uri.find("?");
    string::size_type hash_position = this->uri.find("#");

    if (ampersand_position != string::npos && colon_position != string::npos && colon_position < ampersand_position) {
        this->user = (scheme_delimiter != string::npos) ? this->uri.substr((scheme_delimiter + 3), (colon_position - scheme_delimiter - 3)) : this->uri.substr(0, colon_position);
        this->password = this->uri.substr((colon_position + 1), (ampersand_position - colon_position - 1));

        colon_position = this->uri.find(":", ampersand_position);
    }

    if (ampersand_position == string::npos && scheme_delimiter != string::npos) {
        ampersand_position = (scheme_delimiter + 2);
    }

    if (rightbracket_position != string::npos) {
        colon_position = this->uri.find(":", (rightbracket_position + 1));
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
        if (questionmark_position != string::npos) {
            this->path = this->uri.substr(slash_position + 1, questionmark_position - slash_position - 1);
        } else if (hash_position != string::npos) {
            this->path = this->uri.substr(slash_position + 1, hash_position - slash_position - 1);
        } else {
            this->path = this->uri.substr(slash_position + 1);
        }
    }

    if (questionmark_position != string::npos) {
        if (hash_position != string::npos) {
            this->query = this->uri.substr((questionmark_position + 1), (hash_position - questionmark_position - 1));
        } else {
            this->query = this->uri.substr(questionmark_position + 1);
        }
    }

    if (hash_position != string::npos) {
        this->fragment = this->uri.substr(hash_position + 1);
    }
}

string UriParser::getUri() {
    return this->uri;
}

string UriParser::getScheme() {
    return this->scheme;
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

string UriParser::getQuery() {
    return this->query;
}

string UriParser::getFragment() {
    return this->fragment;
}

