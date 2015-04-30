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

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include <optparse/OptionParser.h>

#include <vector>
#include <map>
#include <string>

#include "IOException.h"

using namespace std;


vector<string> split(const string &, const string &);
map<string, string> asMap(const vector<string> &input);
map<string, string> parse_key_value(const string &, const string &);

class OptionsSetter {
public:
	OptionsSetter(const optparse::Values &options);
	virtual ~OptionsSetter();

	string getContent();

	template<typename T, typename Y>
	void set(const string &name, T *obj, Y setter) const {
		if (options.is_set(name)) {
			const string value = options[name];

			(obj->*setter)(value);
		}
	}



	/**
	 * NOTE: probably there's a better way to do these methods, so this code
	 * should be checked in the future
	 */
	template<typename T, typename Y>
	void setNumber(const string &name, T *obj, Y setter) const {
		if (options.is_set(name)) {
			int value = static_cast<int>(options.get(name));

			(obj->*setter)(value);
		}
	}

	template<typename T, typename Y>
	void setBoolean(const string &name, T *obj, Y setter) const {
		if (options.is_set(name)) {
			const string value = options[name];

			if (value == "yes") {
				(obj->*setter)(true);
			}
			else {
				(obj->*setter)(false);
			}
		}
	}

	template<typename T, typename Y>
	void setMap(const string &name, T *obj, Y setter,
			const string &keySeparator = ";",
			const string &propertySeparator = ":") const
	{
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


private:
	optparse::Values options;

};


#endif /* DTESTS_NODE_DATA_CLIENTS_LANG_CPP_COMMON_OPTIONS_OPTIONSHELPER_H_ */
