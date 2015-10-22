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
#include <proton/data.hpp>
#include <proton/value.hpp>


#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

#include "options/OptionsHelper.h"

using proton::data;
using proton::value;

namespace dtests {
namespace proton {
namespace reactor {

using std::string;

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
    logger(debug) << "Normalizing: " << orig;
    
    dv.encoder() << orig;
   
    
    return dv.decoder().data();
}

/**
 * Default normalizer struct instance
 */
static OptionNormalizer<data &> defaultNormalizer = {
    normalizeFunc,
};

} /* namespace reactor */
} /* namespace proton */
} /* namespace dtests */


#endif /* REACTOR_NORMALIZER_H */

