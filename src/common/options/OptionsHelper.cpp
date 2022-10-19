/*
 * OptionsHelper.cpp
 *
 *  Created on: Apr 21, 2015
 *      Author: opiske
 */

#include <fstream>
#include "OptionsHelper.h"
#include "logger/LoggerWrapper.h"


using namespace dtests::common;
using namespace dtests::common::log;

static bool fileExists(const std::string& name)
{
    std::ifstream f(name);
    return f.is_open();
}

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

        if (!fileExists(contentFile)) {
            logger(error) << "The file " << contentFile << " does"
                    << " not exist";
            throw IOException("The file " + contentFile + " does not exist");
        }

        std::ifstream inputStream(contentFile);

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
