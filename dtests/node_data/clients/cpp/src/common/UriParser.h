/*
 * UriParser.h
 *
 *  Created on: Feb 23, 2017
 *      Author: rkubis
 */

#ifndef URIPARSER_H
#define URIPARSER_H

#include <string>

namespace dtests {
namespace common {

using std::string;

/**
 * Implementation of URI parser.
 */
class UriParser {
  public:
    /**
     * Constructor of empty UriParser object
     */
    UriParser();

    /**
     * Constructor of UriParser object with parsed input URI
     *
     * @param uri input URI to parse
     */
    UriParser(const string uri);

    /**
     * Destructor of UriParser object
     */
    virtual ~UriParser();

    /**
     * Parse input URI
     *
     * @param uri input URI to parse
     */
    void parse(const string uri);

    /**
     * Getter for full input URI
     *
     * @return full input URI
     */
    string getUri();

    /**
     * Getter for scheme parsed from input URI
     *
     * @return scheme parsed from input URI
     */
    string getScheme();

    /**
     * Getter for user parsed from input URI
     *
     * @return user parsed from input URI
     */
    string getUser();

    /**
     * Getter for password parsed from input URI
     *
     * @return password parsed from input URI
     */
    string getPassword();

    /**
     * Getter for host parsed from input URI
     *
     * @return host parsed from input URI
     */
    string getHost();

    /**
     * Getter for port parsed from input URI
     *
     * @return port parsed from input URI
     */
    string getPort();

    /**
     * Getter for path parsed from input URI
     *
     * @return path parsed from input URI
     */
    string getPath();

  protected:
    /**
     * Full input URI
     */
    string uri;

    /**
     * Scheme parsed from input URI
     */
    string scheme;

    /**
     * User parsed from input URI
     */
    string user;

    /**
     * Password parsed from input URI
     */
    string password;

    /**
     * Host parsed from input URI
     */
    string host;

    /**
     * Port parsed from input URI
     */
    string port;

    /**
     * Path parsed from input URI
     */
    string path;

};

} /* namespace common */
} /* namespace dtests */

#endif /* URIPARSER_H */

