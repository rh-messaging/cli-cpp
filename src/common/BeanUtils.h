/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BeanUtils.h
 * Author: opiske
 *
 * Created on October 29, 2015, 12:19 PM
 */

#ifndef BEANUTILS_H
#define BEANUTILS_H

#include <string>
#include <list>
#include <map>

using std::string;
using std::map;
using std::list;


/**
 * An StringOptionNormalizer defines a flexible structure for converting string 
 * types to the different data types used in the messaging APIs.
 */
template<typename K> struct StringOptionNormalizer {
    typedef K(*normalizer)(const string &);
    normalizer normalizerPtr;
};

/**
 * A ListOptionNormalizer defines a flexible structure for converting string 
 * types to the different data types used in the messaging APIs.
 */
template<typename K> struct ListOptionNormalizer {
    typedef K(*normalizer)(const list<string> &);
    normalizer normalizerPtr;
};

/**
 * A MapOptionNormalizer defines a flexible structure for converting string 
 * types to the different data types used in the messaging APIs.
 */
template<typename K> struct MapOptionNormalizer {
    typedef K(*normalizer)(const map<string, string> &);
    normalizer normalizerPtr;
};




#endif /* BEANUTILS_H */

