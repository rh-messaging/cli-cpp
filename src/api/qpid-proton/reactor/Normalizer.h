/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Normalizer.h
 * Author: opiske
 *
 * Created on October 22, 2015, 9:57 AM
 */

#ifndef REACTOR_NORMALIZER_H
#define REACTOR_NORMALIZER_H

#include <string>
#include <list>

#include <proton/types.hpp>
#include <proton/data.hpp>
#include <proton/value.hpp>

#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

#include "options/OptionsHelper.h"

using proton::data;
using proton::value;
using proton::type_id;
using proton::as;
using proton::LIST;

namespace dtests {
namespace proton {
namespace reactor {

using std::string;
using std::list;

using namespace dtests::common;
using namespace dtests::common::log;

/**
 * Default option normalization function that converts basic and std types to 
 * a proton::data object reference.
 */
template<typename T>
static data &normalizeFunc(const T &orig) {
    value dv;

    Logger logger = LoggerWrapper::getLogger();
    logger(debug) << "Using default normalizer: " << orig;

    dv.encoder() << orig;


    return dv.decoder().data();
}

static string unroll(const list<string> &e) {
    string ret;

    for (std::list<string>::const_iterator it = e.begin(); it != e.end(); it++) {
        ret.append(*it);
        ret.append(";");
    }

    return ret;
}

/**
 * Default option normalization function that converts basic and std types to 
 * a proton::data object reference.
 */
static data &normalizeList(const list<string> &orig) {
    value dv;

    Logger logger = LoggerWrapper::getLogger();

    logger(debug) << "Using list normalizer: " << unroll(orig);

    dv.encoder() << as<LIST>(orig);


    return dv.decoder().data();
}

/**
 * Default normalizer struct instance
 */
static StringOptionNormalizer<data &> defaultNormalizer = {
    normalizeFunc,
};


/**
 * Default normalizer struct instance
 */
static ListOptionNormalizer<data &> listNormalizer = {
    normalizeList,
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* REACTOR_NORMALIZER_H */

