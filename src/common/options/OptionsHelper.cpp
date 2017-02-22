/*
 * OptionsHelper.cpp
 *
 *  Created on: Apr 21, 2015
 *      Author: opiske
 */

#include "OptionsHelper.h"
#include "logger/LoggerWrapper.h"


namespace fs = boost::filesystem;

using namespace dtests::common;
using namespace dtests::common::log;



OptionsSetter::OptionsSetter(const optparse::Values &options)
    : options(options)
{
}

OptionsSetter::~OptionsSetter()
{
}

string OptionsSetter::getContent() const
{
    Logger logger = LoggerWrapper::getLogger();

    if (options.is_set("msg-content")) {
        return options["msg-content"];
    } else if(options.is_set("msg-content-from-file")) {
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
    } else {
        return "";
    }
}
