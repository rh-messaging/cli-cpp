PROJECT(dtests-cpp-common)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.4)

include_directories(
	common/
	common/exceptions
	common/options
	common/options/modern
)


add_library(dtests-cpp-common

	exceptions/BaseException.cpp
	exceptions/IOException.cpp
	options/BaseOptionsParser.cpp
	options/OptionsHelper.cpp

	Client.cpp
	Utils.cpp
)

target_link_libraries(dtests-cpp-common
	qpid-proton
	boost_log
	boost_log_setup
	boost_filesystem
	boost_system
	pthread
)

cmake_policy(SET CMP0003 OLD)
add_library(dtests-cpp-common-options

	options/modern/ModernOptionsParser.cpp
	options/modern/SenderOptionsParser.cpp
	options/modern/ReceiverOptionsParser.cpp
)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


target_link_libraries(dtests-cpp-common-options
	dtests-cpp-common
	dtests-cpp-contrib
)
