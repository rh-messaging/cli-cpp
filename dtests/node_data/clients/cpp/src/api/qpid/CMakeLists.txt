PROJECT(dtests-proton-cpp)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.4)

INCLUDE (CheckIncludeFiles)

set(QPID_MESSAGING_DIR /usr CACHE String "QPID Messaging Directory")
set(QPID_INCLUDE_DIR ${QPID_MESSAGING_DIR}/include/qpid CACHE String "QPID Messaging Include Directory")
set(QPID_LIBRARY_DIR ${QPID_MESSAGING_DIR}/lib CACHE String "QPID Messaging Library Directory")
set(QPID_WITH_GET_CONTENT_OBJECT OFF CACHE Bool "QPID Messaging contains 'GetContentObject")
set(QPID_VERSION "0.18" CACHE String "QPID Version") 
set(QPID_SOURCE "/usr/src/debug/qpid-${QPID_VERSION}/cpp" CACHE String "QPID Version")
set(QPID_SOURCE_INCLUDE_DIR "${QPID_SOURCE}/include" CACHE String "QPID source code include dir")
set(ENABLE_QPID_LL OFF CACHE BOOL "Enable qpid low level clients")

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
        common/formatter
)

link_directories(
	${QPID_LIBRARY_DIR}
)

if (ENABLE_QPID_LL)
	set(qpid_low_level_sources
		common/Statistics.cpp
	)
endif (ENABLE_QPID_LL)

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
	boost_regex
	dtests-cpp-legacy
)

# These binaries depend on the qpid source code, therefore they may note be 
# compiled unless qpid-cpp-mrg-debuginfo and qpid-qmf-debuginfo have been 
# installed or the qpid source code is present somehow.
if (ENABLE_QPID_LL) 
    add_definitions(-DENABLE_QPID_LL)

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
endif(ENABLE_QPID_LL)

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
