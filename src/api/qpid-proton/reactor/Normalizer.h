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
using proton::MAP;
using proton::message_id;

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
/*
template<typename T>
static data normalizeFunc(const T &orig) {
    value dv;

    Logger logger = LoggerWrapper::getLogger();
    logger(debug) << "Using default normalizer: " << orig;

    dv = orig;

    return dv.decoder().data();
}
 * */


/**
 * Default option normalization function that converts basic and std types to 
 * a proton::data object reference.
 */
template<typename T>
static message_id normalizeIdFunc(const T &orig) {
    message_id id;

    Logger logger = LoggerWrapper::getLogger();
    logger(debug) << "Using ID normalizer: " << orig;

    id = orig;

    return id;
}

/**
 * Unrolls a string list for debugging purposes
 * @param e entry list
 * @return an unrolled string where each entry in the list is separated by ;
 */
static string unroll(const list<string> &e) {
    string ret;

    
    for (std::list<string>::const_iterator it = e.begin(); it != e.end(); it++) {
        ret.append(*it);
        ret.append(";");
    }
    

    return ret;
}


/**
 * Unrolls a string list for debugging purposes
 * @param e entry list
 * @return an unrolled string where each entry in the list is separated by ;
 */
static string unroll(const map<string, string> &e) {
    string ret;

    ret.append("{");
    for (std::map<string, string>::const_iterator it = e.begin(); it != e.end(); it++) {
        ret.append((*it).first);
        ret.append("=");
        ret.append((*it).second);
        ret.append(";");
    }
    ret.append("}");

    return ret;
}

/**
 * Default option normalization function that converts list types to 
 * a proton::data object reference.
 */


static value normalizeList(const list<string> &orig) {
    value dv;

    Logger logger = LoggerWrapper::getLogger();

    logger(debug) << "Using list normalizer: " << unroll(orig);

    dv = as<LIST>(orig);


    return dv;
}



/**
 * Default option normalization function that converts map types to 
 * a proton::data object reference.
 */
static value normalizeMap(const map<string, string> &orig) {
    value dv;

    Logger logger = LoggerWrapper::getLogger();

    logger(debug) << "Using map normalizer: " << unroll(orig);

    dv = as<MAP>(orig);


    return dv;
}

/**
 * Data normalizer struct instance for string-based data
 */
/*
static StringOptionNormalizer<data> defaultNormalizer = {
    normalizeFunc,
};
 */


/**
 * Data normalizer struct instance for string-based ids
 */
static StringOptionNormalizer<message_id> idNormalizer = {
    normalizeIdFunc,
};


/**
 * Data normalizer struct instance for list-based data
 */

static ListOptionNormalizer<value> listNormalizer = {
    normalizeList,
};



/**
 * Data normalizer struct instance for map-based data
 */
static MapOptionNormalizer<value> mapNormalizer = {
    normalizeMap,
};


} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* REACTOR_NORMALIZER_H */

