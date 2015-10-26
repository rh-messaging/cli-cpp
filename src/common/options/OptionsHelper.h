/*
 * OptionsHelper.h
 *
 *  Created on: Apr 21, 2015
 *      Author: opiske
 */

#ifndef DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_OPTIONS_OPTIONSHELPER_H_
#define DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_OPTIONS_OPTIONSHELPER_H_

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <optparse/OptionParser.h>

#include <vector>
#include <map>
#include <string>
#include <list>

#include "IOException.h"
#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

using namespace std;


vector<string> split(const string &, const string &);
map<string, string> asMap(const vector<string> &input);
map<string, string> parse_key_value(const string &, const string &);

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

/**
 * An utility class template to help set options values/command values to their 
 * respective class instances. For example, given an instance of this class, 
 * a command line option named msg-reply-to and a bean named 'message' of type 
 * Message:
 * 
 * setter.set("msg-reply-to", &message, &Message::setReplyTo);
 *
 * Therefore reducing all the clutter of reading the option value, testing for 
 * empty/null/invalid/etc and setting it on the target object instance
 * 
 */
class OptionsSetter {
  public:
    OptionsSetter(const optparse::Values &options);
    virtual ~OptionsSetter();

    string getContent() const;

    /**
     * Set an option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     */
    template<typename T, typename Y>
    void set(const string &name, T *obj, Y setter) const {
        if (options.is_set(name)) {
            const string value = options[name];

            (obj->*setter)(value);
        }
    }

    /**
     * Set an option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * @param normalizerStruc pointer to an OptionNormalizer that converts 
     * basic types to string to whatever type is used in the setter.
     */
    template<typename T, typename Y, typename K>
    void set(const string &name, T *obj, Y setter,
            const StringOptionNormalizer<K> *normalizerStruc) const {
        if (options.is_set(name)) {
            const string value = options[name];

            K normalizedValue = normalizerStruc->normalizerPtr(value);
            (obj->*setter)(normalizedValue);
        }
    }

    /**
     * Set a number option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * @param defdefaultValue the default value to use if unset
     * NOTE: probably there's a better way to do these methods, so this code
     * should be checked in the future
     */
    template<typename T, typename Y, typename K>
    void setNumber(const string &name, T *obj, Y setter, K defaultValue) const {
        if (options.is_set(name)) {
            int value = static_cast<K> (options.get(name));

            (obj->*setter)(value);
        } else {
            (obj->*setter)(defaultValue);
        }
    }

    /**
     * Set a number option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * NOTE #1: probably there's a better way to do these methods, so this code
     * should be checked in the future
     * NOTE #2: the code uses an integer by default. For other types, 
     * use the setNumber with default values
     */
    template<typename T, typename Y>
    void setNumber(const string &name, T *obj, Y setter) const {
        if (options.is_set(name)) {
            int value = static_cast<int> (options.get(name));

            (obj->*setter)(value);
        }
    }

    /**
     * Set a boolean option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * NOTE: probably there's a better way to do these methods, so this code
     * should be checked in the future
     */
    template<typename T, typename Y>
    void setBoolean(const string &name, T *obj, Y setter) const {
        if (options.is_set(name)) {
            const string value = options[name];

            if (value == "yes") {
                (obj->*setter)(true);
            } else {
                (obj->*setter)(false);
            }
        }
    }

    /**
     * Set a map option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * NOTE: probably there's a better way to do these methods, so this code
     * should be checked in the future
     */
    template<typename T, typename Y>
    void setMap(const string &name, T *obj, Y setter,
            const string &keySeparator = ";",
            const string &propertySeparator = ":") const {
        if (options.is_set(name)) {
            const string property = options[name];

            vector<string> propertyVector = split(property, keySeparator);

            map<string, string> properties = map<string, string>();
            for (size_t i = 0; i < propertyVector.size(); i++) {
                map<string, string> tmp = parse_key_value(propertyVector[i],
                        propertySeparator);

                properties.insert(tmp.begin(), tmp.end());
            }

            (obj->*setter)(properties);
        }
    }
    
    /**
     * Set a map option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * NOTE: probably there's a better way to do these methods, so this code
     * should be checked in the future
     */
    template<typename T, typename Y, typename K>
    void setMap(const string &name, T *obj, Y setter, 
            const MapOptionNormalizer<K> *normalizer,
            const string &keySeparator = ";",
            const string &propertySeparator = ":") const {
        if (options.is_set(name)) {
            const string property = options[name];

            vector<string> propertyVector = split(property, keySeparator);

            map<string, string> properties = map<string, string>();
            for (size_t i = 0; i < propertyVector.size(); i++) {
                map<string, string> tmp = parse_key_value(propertyVector[i],
                        propertySeparator);

                properties.insert(tmp.begin(), tmp.end());
            }

            
            K normalizedValue = normalizer->normalizerPtr(properties);
            (obj->*setter)(normalizedValue);
        }
    }
    
    
    /**
     * Set a list option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * NOTE: probably there's a better way to do these methods, so this code
     * should be checked in the future
     */
    template<typename T, typename Y, typename K>
    void setList(const string &name, T *obj, Y setter,
            const ListOptionNormalizer<K> *normalizerStruct,
            const string &keySeparator = ";") const {
        if (options.is_set(name)) {
            const string property = options[name];

            vector<string> propertyVector = split(property, keySeparator);

            
            list<string> entries = list<string>();
            for (size_t i = 0; i < propertyVector.size(); i++) {
                entries.push_back(propertyVector[i]);
            }

             K normalizedValue = normalizerStruct->normalizerPtr(entries);
            (obj->*setter)(normalizedValue);
        }
    }


  private:
    optparse::Values options;

};


#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_OPTIONS_OPTIONSHELPER_H_ */
