PROJECT(dtests-proton-cpp)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.4)

set(QPID_MESSAGING_DIR /usr CACHE String "QPID Messaging Directory")

include_directories(
	${QPID_MESSAGING_DIR}/include
	common
)

link_directories(
	${QPID_MESSAGING_DIR}/lib64
)

# TODO: move this to a common directory
cmake_policy(SET CMP0003 OLD)
add_library(
	dtests-qpid-messaging-common

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


foreach(client ${qpid_messaging_clients})

	add_executable(${client}
		messaging/${client}.cpp
	)


	target_link_libraries(${client}
		dtests-qpid-messaging-common
	)

endforeach(client)
