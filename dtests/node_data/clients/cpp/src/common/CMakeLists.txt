PROJECT(dtests-cpp-common)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.4)

# CHECK_INCLUDE_FILE(qpid/types/Exception.h HAVE_QPID_TYPES_EXCEPTION)
# FailIfNotSet(HAVE_QPID_TYPES_EXCEPTION qpid/types/Exception.h)

include_directories(
	common/
	common/exceptions
	common/options
	common/options/modern
)



if (ENABLE_MODERN) 
    set(COMMON_MODERN_SRC
        output/Printer.cpp            
        exceptions/BaseException.cpp
	exceptions/IOException.cpp
	exceptions/TimerExpiredException.cpp
	options/BaseOptionsParser.cpp
	options/OptionsHelper.cpp
        formatter/Writer.cpp
        formatter/DictWriter.cpp
        formatter/UpstreamWriter.cpp
        formatter/AbstractDecoder.cpp
        formatter/AbstractFormatter.cpp
        formatter/DictFormatter.cpp
        formatter/UpstreamFormatter.cpp
        logger/Logger.cpp
        logger/LoggerWrapper.cpp

	Timer.cpp
	UriParser.cpp
	Client.cpp
	ContentType.cpp
        MessageHeader.cpp
    )

    if (BUILD_WITH_UNIT_TESTS)
        set(TEST_SUPPORT_SRC
            TestUtils.cpp
        )

	add_subdirectory(test)
        add_subdirectory(output/test)
        add_subdirectory(logger/test)
        add_subdirectory(options/test)
     endif (BUILD_WITH_UNIT_TESTS)
        
endif (ENABLE_MODERN)

add_library(dtests-cpp-common
        ${COMMON_MODERN_SRC}
        ${TEST_SUPPORT_SRC}

	Utils.cpp
)

target_link_libraries(dtests-cpp-common
	boost_filesystem
	boost_system
	pthread
)

cmake_policy(SET CMP0003 OLD)
add_library(dtests-cpp-common-options

	options/modern/StringAppendCallback.cpp
	options/modern/ModernOptionsParser.cpp
	options/modern/SenderOptionsParser.cpp
	options/modern/ReceiverOptionsParser.cpp
	options/modern/ConnectorOptionsParser.cpp
)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


target_link_libraries(dtests-cpp-common-options
	dtests-cpp-common
	dtests-cpp-contrib
)


install(TARGETS dtests-cpp-common dtests-cpp-common-options
	LIBRARY DESTINATION ${CPP_LIBRARY_DIR}
	RUNTIME DESTINATION ${CPP_LIBRARY_DIR}
        ARCHIVE DESTINATION ${CPP_LIBRARY_DIR}
)


InstallHeaders(src/common common)
InstallHeaders(src/common/exceptions common/exceptions)
InstallHeaders(src/common/formatter common/formatter)
InstallHeaders(src/common/logger common/logger)
InstallHeaders(src/common/options common/options)
InstallHeaders(src/common/options/modern common/options/modern)
InstallHeaders(src/common/output common/output)
