PROJECT(dtests-proton-cpp)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.8.12)

set(PROTON_DIR /usr CACHE String "QPID Proton base directory")

# Do not change. Please see the comments on the aac{1,2}_* clients
set(QPID_PROTON_WITH_MESSAGING_API OFF CACHE BOOL 
    "Uses the messaging API instead of the reactor API")

if (QPID_PROTON_WITH_MESSAGING_API)
    message(WARNING "Building with the messaging API instead of the reactor API")
    add_definitions(-DQPID_PROTON_WITH_MESSAGING_API) 
endif(QPID_PROTON_WITH_MESSAGING_API)

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

if (WIN32)
    target_link_libraries(
        dtests-proton-common

        dtests-cpp-contrib
        dtests-cpp-common
        dtests-cpp-common-options
    )
else (WIN32)
    target_link_libraries(
        dtests-proton-common

        boost_regex
        dtests-cpp-contrib
        dtests-cpp-common
        dtests-cpp-common-options
    )
endif (WIN32)

cmake_policy(SET CMP0003 OLD)
add_library(
    dtests-proton-reactor
    
    reactor/formatter/ReactorDecoder.cpp
    reactor/handler/CommonHandler.cpp
    reactor/handler/ConnectorHandler.cpp
    reactor/handler/SenderHandler.cpp
    reactor/handler/ReceiverHandler.cpp
    reactor/ConnectingClient.cpp
    reactor/SendingClient.cpp
    reactor/ReceivingClient.cpp
)

target_link_libraries(
    dtests-proton-reactor

    dtests-cpp-contrib
    dtests-cpp-common
    dtests-cpp-common-options
    dtests-proton-common

    qpid-proton-cpp
)

set(proton-reactor-clients
    aac3_connector
    aac3_sender
    aac3_receiver
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

install(TARGETS 
    dtests-proton-common 
    dtests-proton-reactor
    ${proton-reactor-clients} 
    LIBRARY DESTINATION ${CPP_LIBRARY_DIR}
    RUNTIME DESTINATION ${CPP_RUNTIME_DIR}
    ARCHIVE DESTINATION ${CPP_LIBRARY_DIR}
)

InstallHeaders(src/api/qpid-proton qpid-proton)
InstallHeaders(src/api/qpid-proton/common qpid-proton/common)
InstallHeaders(src/api/qpid-proton/reactor qpid-proton/reactor)
InstallHeaders(src/api/qpid-proton/reactor/formatter qpid-proton/reactor/formatter)
InstallHeaders(src/api/qpid-proton/reactor/handler qpid-proton/reactor/handler)
