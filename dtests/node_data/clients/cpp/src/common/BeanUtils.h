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

#ifndef DTESTS_COMMON_BEANUTILS_H
#define DTESTS_COMMON_BEANUTILS_H

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

class BeanUtils {
  public:
    /**
     * Set an option to a class instance (bean)
     * @param value the value to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     */
    template<typename V, typename T, typename Y>
    static void set(const V &value, T *obj, Y setter) {
        (obj->*setter)(value);
    }
    
    
    /**
     * Set an option to a class instance (bean)
     * @param value the value to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * @param normalizerStruc pointer to an OptionNormalizer that converts 
     * basic types to string to whatever type is used in the setter.
     */
    template<typename T, typename Y, typename K>
    static void set(const string &value, T *obj, Y setter,
        const StringOptionNormalizer<K> *normalizerStruc) {

        K normalizedValue = normalizerStruc->normalizerPtr(value);
        (obj->*setter)(normalizedValue); 
    }
    
   /**
     * Set a boolean option to a class instance (bean)
     * @param value the value to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * NOTE: probably there's a better way to do these methods, so this code
     * should be checked in the future
     */
    template<typename T, typename Y>
    static void setBoolean(const string &value, T *obj, Y setter) {
        if (value == "yes") {
            (obj->*setter)(true);
        } else {
            (obj->*setter)(false);
        }
    }
    
};

#endif /* DTESTS_COMMON_BEANUTILS_H */

