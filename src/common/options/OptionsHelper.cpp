/*
 * OptionsHelper.cpp
 *
 *  Created on: Apr 21, 2015
 *      Author: opiske
 */

#include "OptionsHelper.h"
#include "logger/LoggerWrapper.h"

namespace algo = boost::algorithm;
namespace fs = boost::filesystem;

using namespace dtests::common::log;

using dtests::common::IOException;

vector<string> split(const string &input, const string &sep)
{
    vector<string> splitted;

    algo::split(splitted, input, algo::is_any_of(sep),
            algo::token_compress_on);

    return splitted;
}

map<string, string> asMap(const vector<string> &input)
{
    map<string, string> ret = map<string, string>();

    for (size_t i = 0; i < (input.size() - 1); i = i + 2) {
        string key = input[i];
        string value = input[i + 1];

        ret[key] = value;
    }

    return ret;
}

map<string, string> parse_key_value(const string &input, const string &sep)
{
    vector<string> splitted;

    algo::split(splitted, input, algo::is_any_of(sep),
            algo::token_compress_on);

    return asMap(splitted);
}

OptionsSetter::OptionsSetter(const optparse::Values &options)
    : options(options)
{
}

OptionsSetter::~OptionsSetter()
{
}

string OptionsSetter::getContent()
{
    Logger logger = LoggerWrapper::getLogger();

    if (options.is_set("msg-content")) {
        return options["msg-content"];
    } else {
        const string contentFile = options["msg-content-from-file"];

        fs::path file(contentFile);

        if (!exists(file)) {
            logger(error) << "The file " << contentFile << " does"
                    << " not exist";
            throw IOException("The file " + contentFile + " does not exist");
        }

        fs::ifstream inputStream(file);

        string content;

        while (!inputStream.eof()) {
            inputStream >> content;
        }

        logger(debug) << "Read from file " << content;

        return content;
    }
}
