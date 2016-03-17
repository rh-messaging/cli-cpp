/*
 * OptionsHelper.h
 *
 *  Created on: Apr 21, 2015
 *      Author: opiske
 */

#ifndef DTESTS_COMMON_OPTIONSHELPER_H_
#define DTESTS_COMMON_OPTIONSHELPER_H_

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
#include "BeanUtils.h"
#include "STLUtils.h"
#include "logger/Logger.h"
#include "logger/LoggerWrapper.h"

using namespace std;

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



namespace dtests {
namespace common {

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

            BeanUtils::set(value, obj, setter);
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
            
            BeanUtils::set(value, obj, setter, normalizerStruc);
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
            K value = static_cast<K> (options.get(name));

            BeanUtils::set(value, obj, setter);
        } else {
            BeanUtils::set(defaultValue, obj, setter);
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

            BeanUtils::set(value, obj, setter);
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
            
            BeanUtils::setBoolean(value, obj, setter);
        }
    }

    /**
     * Set a map option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * @param keySeparator the separator string between elements in the key 
     * @param propertySeparator the separator string between elements in the 
     * properties 
     * NOTE2: this method takes an input string in the format 
     * "key1:123;key2:764;key3:=999;key4:longervalue" and splits twice to 
     * decompose a key value pair stored in a map
     */
    template<typename T, typename Y>
    void setMap(const string &name, T *obj, Y setter,
            const string &keySeparator = ";",
            const string &propertySeparator = ":") const {
        if (options.is_set(name)) {
            const string property = options[name];

            vector<string> propertyVector = split(property, keySeparator);
            map<string, string> properties = map<string, string>();
             convertPropertiesToMap(propertyVector, properties, 
                     propertySeparator);
          
            BeanUtils::set(properties, obj, setter);
        }
    }
    
    /**
     * Set a map option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * @param output the output map
     * @param keySeparator the separator string between elements in the key 
     * @param propertySeparator the separator string between elements in the 
     * properties 
     * NOTE2: this method takes an input string in the format 
     * "key1:123;key2:764;key3:=999;key4:longervalue" and splits twice to 
     * decompose a key value pair stored in a map
     */
    template<typename K>
    void setMap(const string &name, map<string, K> &output,
            const string &keySeparator = ";",
            const string &propertySeparator = ":") const {
        if (options.is_set(name)) {
            const string property = options[name];

            vector<string> propertyVector = split(property, keySeparator);
            convertPropertiesToMap(propertyVector, output, propertySeparator);
        }
    }
    
    
    /**
     * Set a map option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * @param normalizer a normalization structure to convert the data from 
     * string/string to the appropriate types used by the underlying API
     * @param keySeparator the separator string between elements in the key 
     * @param propertySeparator the separator string between elements in the 
     * properties 
     * NOTE: this method takes an input string in the format 
     * "key1:123;key2:764;key3:=999;key4:longervalue" and splits twice to 
     * decompose a key value pair stored in a map
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
             convertPropertiesToMap(propertyVector, properties, 
                     propertySeparator);
            
            K normalizedValue = normalizer->normalizerPtr(properties);
            (obj->*setter)(normalizedValue);
        }
    }
    
        
    
    /**
     * Set a list option to a class instance (bean)
     * @param name the option to set
     * @param obj the bean (object) to set the option to
     * @param setter the (bean) setter to set the option
     * @param normalizerStruct a normalization structure to convert the data from 
     * string/string to the appropriate types used by the underlying API
     * @param keySeparator the separator string between elements in the key 
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

} /* namespace common */
} /* namespace dtests */

#endif /* DTESTS_COMMON_OPTIONSHELPER_H_ */
