PROJECT(dtests-proton-cpp)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.8.12)

INCLUDE (CheckIncludeFiles)

set(QPID_MESSAGING_DIR /usr CACHE String "QPID Messaging Directory")
set(QPID_INCLUDE_DIR ${QPID_MESSAGING_DIR}/include/qpid CACHE String "QPID Messaging include directory")
set(QPID_LIBRARY_DIR ${QPID_MESSAGING_DIR}/lib CACHE String "QPID Messaging library directory")
set(QPID_WITH_GET_CONTENT_OBJECT OFF CACHE Bool "Whether qpid code contains getContentObjectQPID 'getContentObject'")
set(QPID_VERSION "0.18" CACHE String "QPID version") 
set(QPID_SOURCE "/usr/src/debug/qpid-${QPID_VERSION}/cpp" CACHE String "QPID source code directory (required for building QPID's client)")
set(QPID_SOURCE_INCLUDE_DIR "${QPID_SOURCE}/include" CACHE String "QPID source code include directory (required for building QPID's client)")
set(ENABLE_QPID_CLIENT OFF CACHE BOOL "Enable QPID clients (qpid-send, qpid-receive and its qc2-* counterpars)")

set(QPID_SOURCE_WITH_SYS_EPOCH ON CACHE BOOL "Use QPIDSYSEPOCH")


if (QPID_WITH_GET_CONTENT_OBJECT) 
    add_definitions(-DNGETCONTENTOBJECT)
endif (QPID_WITH_GET_CONTENT_OBJECT) 


if (${DTEST_BUILD_PLATFORM} EQUAL "64")
	set(QPID_LIBRARY_DIR ${QPID_LIBRARY_DIR}64 )
endif(${DTEST_BUILD_PLATFORM} EQUAL "64")



include_directories(
	${QPID_INCLUDE_DIR}
	.
	common
)

link_directories(
	${QPID_LIBRARY_DIR}
)

if (ENABLE_QPID_CLIENT)
        set(qpid_client_link_libraries
            qpidcommon
        )

	set(qpid_low_level_sources
		common/Statistics.cpp
	)
endif (ENABLE_QPID_CLIENT)

# TODO: move this to a common directory
cmake_policy(SET CMP0003 OLD)
add_library(
	dtests-qpid-messaging-common

        common/formatter/QpidDecoder.cpp
        common/formatter/FormatUtil.cpp
	${qpid_low_level_sources}
        
        # legacy code that should not be used
        common/formatter/Formatter.cpp

        # TODO: make reusable
	messaging/BrokerAgent.cpp
	
)

target_link_libraries(
	dtests-qpid-messaging-common

	qpidmessaging
	qpidtypes
	dtests-cpp-legacy
	boost_regex  # would be `Boost::regex` in new CMake
	${qpid_client_link_libraries}
)

# These binaries depend on the qpid source code, therefore they may note be 
# compiled unless qpid-cpp-mrg-debuginfo and qpid-qmf-debuginfo have been 
# installed or the qpid source code is present somehow.
if (ENABLE_QPID_CLIENT) 
    add_definitions(-DENABLE_QPID_CLIENT)

    include_directories(
	${QPID_SOURCE_INCLUDE_DIR}
    )

    check_include_files(HAVE_QPID_SYS_TIME qpid/sys/Time.h)

    if (QPID_SOURCE_WITH_SYS_EPOCH)
        add_definitions(-DQPIDSYSEPOCH)
    else (QPID_SOURCE_WITH_SYS_EPOCH)
        add_definitions(-DQPIDSYSDURATIONFROMEPOCH)
    endif (QPID_SOURCE_WITH_SYS_EPOCH)

    set(qpid_messaging_ll_clients
        qc2_qpid-receive
        qc2_qpid-send
        qpid-receive
        qpid-send
    )

    target_link_libraries(
	qpidclient
    )
endif(ENABLE_QPID_CLIENT)

set(qpid_messaging_clients
	qc2_connector
	qc2_drain
	qc2_map_receiver
	# TODO: requires <qpid/Options.h>
	# qc2_qpid-receive

	# TODO: requires <qpid/sys/Time.h>
	# qc2_qpid-send
	qc2_server_example
	qc2_spout

	# TODO: requires <qpid/Options.h>
	# qc2_txtest
	qmf2_list_objects
	qmf2_qpid_ctrl

	# TODO: requires <qpid/Options.h>
	# qpid-receive

	# TODO: requires <qpid/sys/Time.h>
	# qpid-send

        ${qpid_messaging_ll_clients}

        aac0_connector
        aac0_receiver
        aac0_sender
)



# This is for the legacy code
add_library(dtests-cpp-legacy

	common/OptionParser.cpp
)

target_link_libraries(dtests-cpp-legacy
	qpidmessaging
	dtests-cpp-common
)

foreach(client ${qpid_messaging_clients})

	add_executable(${client}
		messaging/${client}.cpp
	)


	target_link_libraries(${client}
		dtests-qpid-messaging-common
	)

endforeach(client)


if (ENABLE_MODERN) 
    if (BUILD_WITH_UNIT_TESTS)
        add_subdirectory(common/formatter/test)
     endif (BUILD_WITH_UNIT_TESTS)
endif (ENABLE_MODERN)


install(TARGETS 
            dtests-qpid-messaging-common
            dtests-cpp-legacy
            ${qpid_messaging_clients}
           
	LIBRARY DESTINATION ${CPP_LIBRARY_DIR}
	RUNTIME DESTINATION ${CPP_RUNTIME_DIR}
        ARCHIVE DESTINATION ${CPP_LIBRARY_DIR}
)


InstallHeaders(src/api/qpid qpid)
InstallHeaders(src/api/qpid/common qpid/common)
InstallHeaders(src/api/qpid/common/formatter qpid/common/formatter)
InstallHeaders(src/api/qpid/messaging qpid/messaging)
