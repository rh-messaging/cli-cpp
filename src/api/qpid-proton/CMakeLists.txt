PROJECT(dtests-proton-cpp)

set(PROTON_DIR /usr CACHE String "QPID Proton base directory")

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

if (UNIX AND ProtonCpp_VERSION VERSION_GREATER_EQUAL 0.38.0)
    set(OPENTELEMETRY_ENABLED ON)
    set(MODERNClient_tracing_src common/ModernClient_tracing_opentracing.cpp)
else ()
    set(OPENTELEMETRY_ENABLED OFF)
    set(MODERNClient_tracing_src common/ModernClient_tracing_none.cpp)
endif ()


cmake_policy(SET CMP0003 OLD)
add_library(
    dtests-proton-common
    
    common/ModernClient.cpp
    ${MODERNClient_tracing_src}
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

    # Proton::cpp
    qpid-proton-cpp
)

if (OPENTELEMETRY_ENABLED)
    set(CMAKE_BUILD_TYPE "")
    find_package(opentelemetry-cpp)
    include_directories(${OPENTELEMETRY_CPP_INCLUDE_DIRS})

    target_link_libraries(dtests-proton-reactor
            opentelemetry-cpp::trace
            opentelemetry-cpp::otlp_http_exporter
    )
endif ()

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
