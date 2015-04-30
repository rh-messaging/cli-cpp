/*
 * Data.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: opiske
 */

#include "Data.h"

namespace dtests {
namespace proton {
namespace messenger {

Data::Data(size_t capacity)
	: super(),
	  data(NULL),
	  release(true)
{
	init(capacity);

}

Data::Data(pn_data_t *refData)
	: super(),
	  data(refData),
	  release(false)
{

}

Data::Data(const string& string)
	: super(),
	  data(NULL),
	  release(true)
{
	init(string.length());

	writeString(string);
}

Data::~Data() {
	if (release) {
		BOOST_LOG_TRIVIAL(trace) << "Auto releasing internal data pointer";

		pn_data_free(data);
	}
}

void Data::init(size_t capacity) {
	BOOST_LOG_TRIVIAL(trace) << "Trying to create a new data storage of size "
						<< capacity;

	data = pn_data(capacity);
	if (data == NULL) {

		throw std::bad_alloc();
	}
}


void Data::handleError(const string& message) {
	pn_error_t *dataError = pn_data_error(data);

	throw ProtonError::handleError<ProtonException>(dataError,
					"Unable to write data");
}


void Data::writeString(const string& string) {
	pn_bytes_t bytes = pn_bytes(string.length(), string.c_str());

	int ret = pn_data_put_string(data, bytes);
	if (ret != 0) {
		handleError("Unable to write data");
	}
}

string Data::readString() const {
	char buffer[1024] = {0};

	size_t buffsize = sizeof(buffer);

	pn_type_t type = pn_data_type(data);
	switch (type) {
			case PN_ARRAY: {
				BOOST_LOG_TRIVIAL(debug) << "Processing array";
				break;
			}
			case PN_LIST: {
				BOOST_LOG_TRIVIAL(debug) << "Processing list";
				break;
			}
			case PN_MAP: {
				BOOST_LOG_TRIVIAL(debug) << "Processing map";
				break;
			}
			case PN_STRING: {
				BOOST_LOG_TRIVIAL(debug) << "Processing string";

				pn_bytes_t bytes = pn_data_get_string(data);
				if (bytes.size > sizeof(buffer)) {
					BOOST_LOG_TRIVIAL(warning) << "Data size is bigger than " <<
							"the storage buffer, therefore some content may " <<
							"be lost";
				}

				break;
			}
			default: {
				BOOST_LOG_TRIVIAL(warning) << "Unknown data type: " << type;
				break;
			}
	}

	pn_data_format(data, buffer, &buffsize);
	BOOST_LOG_TRIVIAL(debug) << "Content: " << buffer;

	return string(buffer);
}

void Data::writeData(const Data& src) {
	int ret = pn_data_copy(data, src.data);

	if (ret != 0) {
		handleError("Unable to copy data from the source");
	}
}

bool Data::isEmpty() const {
	int size = pn_data_size(data);

	if (size == 0) {
		return true;
	}

	return false;
}

int Data::size() const {
	return pn_data_size(data);
}


void Data::writeMap(const map<string, string>& map) {
	pn_data_put_map(data);
	pn_data_enter(data);

	for (std::map<string, string>::const_iterator i = map.begin(); i != map.end(); i++) {
		writeString(i->first);
		writeString(i->second);
	}

	pn_data_exit(data);
}

map<string, string> Data::readMap() const {
	map<string, string> ret = map<string, string>();
	size_t size = pn_data_get_map(data);

	pn_data_rewind(data);
	for (size_t i = 0; i < size; i++) {
		string key = readString();
		string value = readString();

		ret[key] = value;
		pn_data_next(data);
	}
	pn_data_exit(data);

	return ret;
}



void Data::writeList(const list<string>& list) {
	pn_type_t type = pn_data_get_array_type(data);

	size_t arraySize = pn_data_get_array(data);

	switch (type) {
		case PN_ARRAY: {
			BOOST_LOG_TRIVIAL(debug) << "Processing array";
			break;
		}
		case PN_LIST: {
			BOOST_LOG_TRIVIAL(debug) << "Processing list";
			break;
		}
		case PN_MAP: {
			BOOST_LOG_TRIVIAL(debug) << "Processing map";
			break;
		}
		default: {
			BOOST_LOG_TRIVIAL(warning) << "Unknown data type: " << type;
			break;
		}
	}
}

list<string> Data::readList() const {
	pn_type_t type = pn_data_type(data);

	switch (type) {
		case PN_ARRAY: {
			BOOST_LOG_TRIVIAL(debug) << "Processing array";
			break;
		}
		case PN_LIST: {
			BOOST_LOG_TRIVIAL(debug) << "Processing list";
			break;
		}
		case PN_MAP: {
			BOOST_LOG_TRIVIAL(debug) << "Processing map";
			break;
		}
		default: {
			BOOST_LOG_TRIVIAL(warning) << "Unknown data type: " << type;
			break;
		}
	}

	return list<string>();
}

}
}
}
