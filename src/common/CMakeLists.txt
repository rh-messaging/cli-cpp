PROJECT(dtests-cpp-common)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.4)

include_directories(
	common/
	common/exceptions
	common/options
	common/options/modern
)

if (ENABLE_MODERN) 
    set(COMMON_MODERN_SRC
        exceptions/BaseException.cpp
	exceptions/IOException.cpp
	options/BaseOptionsParser.cpp
	options/OptionsHelper.cpp
        formatter/Writer.cpp
        formatter/StringWriter.cpp
        formatter/AbstractDecoder.cpp
        formatter/AbstractFormatter.cpp

	Client.cpp
	ContentType.cpp
        MessageHeader.cpp
    )
        
endif (ENABLE_MODERN)

if (ENABLE_PROTON)
    set(COMMON_PROTON_LIBRARIES
        qpid-proton
    )
endif(ENABLE_PROTON)

add_library(dtests-cpp-common
        ${COMMON_MODERN_SRC}

	Utils.cpp
)

if (ENABLE_BOOST_LOG) 
    set(BOOST_LOG_LIBRARIES
        boost_log
	boost_log_setup
    )
endif(ENABLE_BOOST_LOG)

target_link_libraries(dtests-cpp-common
	${COMMON_PROTON_LIBRARIES}
        ${BOOST_LOG_LIBRARIES}
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
