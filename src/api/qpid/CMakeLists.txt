PROJECT(dtests-proton-cpp)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.4)

set(QPID_MESSAGING_DIR /usr CACHE String "QPID Messaging Directory")
set(QPID_INCLUDE_DIR ${QPID_MESSAGING_DIR}/include/qpid CACHE String "QPID Messaging Include Directory")
set(QPID_LIBRARY_DIR ${QPID_MESSAGING_DIR}/lib64 CACHE String "QPID Messaging Library Directory")

include_directories(
	${QPID_INCLUDE_DIR}
	
	common
)

link_directories(
	${QPID_LIBRARY_DIR}/lib64
)

# TODO: move this to a common directory
cmake_policy(SET CMP0003 OLD)
add_library(
	dtests-qpid-messaging-common

        common/formatter/QpidDecoder.cpp

        # TODO: make reusable
	messaging/BrokerAgent.cpp
	messaging/Formatter.cpp
)

target_link_libraries(
	dtests-qpid-messaging-common

	qpidmessaging
	qpidtypes
	boost_regex
	dtests-cpp-legacy
)

set(
	qpid_messaging_clients
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
