PROJECT(dtests-proton-cpp)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.4)

set(PROTON_DIR /usr CACHE String "Proton directory")

include_directories(
	${PROTON_DIR}/include
	common
)

link_directories(
	${PROTON_DIR}/lib64
)

cmake_policy(SET CMP0003 OLD)
add_library(
	dtests-proton-common
	
	common/ModernClient.cpp
)

target_link_libraries(
	dtests-proton-common

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
	qpid-proton-cpp
)


# Qpid Proton Reactor clients
cmake_policy(SET CMP0003 OLD)
add_executable(aac1_sender
	clients/aac1_sender.cpp
)

target_include_directories(
	aac1_sender PUBLIC
	reactor
)

	
target_link_libraries(aac1_sender
	dtests-proton-reactor
)

#cmake_policy(SET CMP0003 OLD)
# add_executable(aac1_receiver
#	clients/aac1_receiver.cpp)
	
#target_link_libraries(aac1_receiver
#	dtests-proton-reactor
#)


# Qpid Proton Messenger clients
cmake_policy(SET CMP0003 OLD)
add_executable(aac2_sender
	clients/aac2_sender.cpp
)

target_include_directories(
	aac2_sender PUBLIC
	messenger
	messenger/exceptions
)
	
target_link_libraries(aac2_sender
	dtests-proton-messenger
)

cmake_policy(SET CMP0003 OLD)
add_executable(aac2_receiver
	clients/aac2_receiver.cpp
)

target_include_directories(
	aac2_receiver PUBLIC
	messenger
	messenger/exceptions
)
	
target_link_libraries(aac2_receiver
	dtests-proton-messenger
)