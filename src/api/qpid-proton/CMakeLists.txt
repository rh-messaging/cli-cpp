PROJECT(dtests-proton-cpp)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.4)


set(PROTON_DIR /usr CACHE String "QPID Proton base directory")

# Do not change. Please see the comments on the aac{1,2}_* clients
set(QPID_PROTON_WITH_MESSAGING_API OFF CACHE BOOL 
    "Uses the messaging API instead of the reactor API")

if (QPID_PROTON_WITH_MESSAGING_API)
    message(WARNING "Building with the messaging API instead of the reactor API")
    add_definitions(-DQPID_PROTON_WITH_MESSAGING_API) 
endif(QPID_PROTON_WITH_MESSAGING_API)

if (QPID_PROTON_WITH_REACTOR_API)
endif (QPID_PROTON_WITH_REACTOR_API)

include_directories(
	${PROTON_DIR}/include
	common
        .
)

if (${DTEST_BUILD_PLATFORM} EQUAL "64")
	link_directories(
		${PROTON_DIR}/lib64
	)
else(${DTEST_BUILD_PLATFORM} EQUAL "64")
	link_directories(
		${PROTON_DIR}/lib
	)
endif(${DTEST_BUILD_PLATFORM} EQUAL "64")

cmake_policy(SET CMP0003 OLD)
add_library(
	dtests-proton-common
	
	common/ModernClient.cpp
)

target_link_libraries(
	dtests-proton-common

	qpid-proton
	dtests-cpp-contrib
	dtests-cpp-common
	dtests-cpp-common-options
)



cmake_policy(SET CMP0003 OLD)
add_library(
	dtests-proton-messenger
	
	messenger/exceptions/ProtonException.cpp
	messenger/exceptions/DataSerializationException.cpp
	
	messenger/ProtonObject.cpp
	messenger/Data.cpp
	messenger/Message.cpp
	messenger/Messenger.cpp
	messenger/SendingClient.cpp
	messenger/ReceivingClient.cpp
)

target_link_libraries(
	dtests-proton-messenger

	dtests-cpp-contrib
	dtests-cpp-common
	dtests-cpp-common-options
	dtests-proton-common
)

cmake_policy(SET CMP0003 OLD)
add_library(
	dtests-proton-reactor
	
	reactor/SimpleMessagingHandler.cpp
	reactor/SendingClient.cpp
)

target_link_libraries(
	dtests-proton-reactor

	dtests-cpp-contrib
	dtests-cpp-common
	dtests-cpp-common-options
	dtests-proton-common
        # Temporarily disabled because this is not yet available
	# qpid-proton-cpp
)


set(proton-reactor-clients
	aac3_sender
	aac3_receiver
)

set(proton-messenger-clients
	aac4_sender
        aac4_receiver
)

 
foreach(client ${proton-reactor-clients})
        cmake_policy(SET CMP0003 OLD)
        add_executable(${client}
                clients/${client}.cpp
        )


        target_link_libraries(${client}
                dtests-proton-reactor
        )
endforeach(client)


foreach(client ${proton-messenger-clients})
        cmake_policy(SET CMP0003 OLD)
	add_executable(${client}
		clients/${client}.cpp
	)


	target_link_libraries(${client}
		dtests-proton-messenger
	)
endforeach(client)

