PROJECT(dtests-cpp-contrib)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.8.12)

INCLUDE (CheckIncludeFile)
INCLUDE (CheckIncludeFileCXX)

macro (FailIfNotSet VARIABLE name)
    if (NOT ${VARIABLE})
        set(MSG "** A required include file, " ${name} ", was not found on your system **")
        message(FATAL_ERROR ${MSG})
    endif (NOT ${VARIABLE})
endmacro(FailIfNotSet)

CHECK_INCLUDE_FILE_CXX(cstdio HAVE_CSTDIO)
FailIfNotSet(HAVE_CSTDIO cstdio)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/lib)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "64 bits compiler detected")
    set(DTEST_BUILD_PLATFORM 64)
    set(DTEST_BUILD_PLATFORM_NAME "x64")

    add_definitions(-DENABLE_IMPLICIT_CONVERSIONS)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "32 bits compiler detected")
    set(DTEST_BUILD_PLATFORM 32)
    set(DTEST_BUILD_PLATFORM_NAME "i686")

    message(STATUS "Implicit conversions are disabled")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

if (WIN32)
    if (EXISTS "C:/Proton/qpid-proton-winsdk/2015")
        set(Boost_INCLUDE_DIRS "C:/Boost2015" CACHE String "Boost include directory")
        set(Boost_LIBRARY_DIRS "C:/Boost2015/lib64-msvc-14.0" CACHE String "Boost library directory")
    else()
        set(Boost_INCLUDE_DIRS "C:/Boost" CACHE String "Boost include directory")
        set(Boost_LIBRARY_DIRS "C:/Boost/lib64-msvc-12.0" CACHE String "Boost library directory")
    endif()

    set(PROTON_INCLUDE_DIR "C:/Proton/qpid-proton-winsdk/include" CACHE String "Proton include directory")
    set(PROTON_LIBRARY_DIR "C:/Proton/qpid-proton-winsdk/lib" CACHE String "Proton library directory")
else (WIN32)
    include(FindBoost)
    find_package(Boost
        1.41.0  # CentOS/RHEL 6
        REQUIRED
        COMPONENTS filesystem regex system
        )
endif (WIN32)

set(ENABLE_QPID_PROTON ON CACHE BOOL "Enable qpid proton clients")
set(ENABLE_QPID_MESSAGING ON CACHE BOOL "Enable qpid messaging clients")
set(ENABLE_MODERN ON CACHE BOOL "Enable usage of refactored code")
set(BUILD_WITH_UNIT_TESTS OFF CACHE BOOL "Build unit tests for the CPP client code")

if (NOT WIN32)
    if (ENABLE_QPID_MESSAGING)
        find_package(Qpid REQUIRED)
    endif (ENABLE_QPID_MESSAGING)
    find_package(Proton REQUIRED Core Proactor)
    find_package(ProtonCpp REQUIRED)
    include_directories(
        ${ProtonCpp_INCLUDE_DIRS}
        ${Qpid_INCLUDE_DIRS}
    )
    link_libraries(
        ${Proton_Core_LIBRARIES}
        ${Proton_Proactor_LIBRARIES}
        ${ProtonCpp_LIBRARIES}
        ${Qpid_LIBRARIES}
    )
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif (NOT WIN32)

if (EXISTS "${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
else (EXISTS "${Boost_INCLUDE_DIRS}")
    message(FATAL_ERROR "Boost ${BOOST_VERSION} headers were not found in ${Boost_INCLUDE_DIRS}")
endif (EXISTS "${Boost_INCLUDE_DIRS}")

if (EXISTS "${Boost_LIBRARY_DIRS}")
    link_directories(${Boost_LIBRARY_DIRS})
else (EXISTS "${Boost_LIBRARY_DIRS}")
    message(FATAL_ERROR "Boost ${BOOST_VERSION} libraries were not found in ${Boost_LIBRARY_DIRS}")
endif (EXISTS "${Boost_LIBRARY_DIRS}")

if (WIN32)
    if (EXISTS "${PROTON_INCLUDE_DIR}")
        include_directories(${PROTON_INCLUDE_DIR})
    else (EXISTS "${PROTON_INCLUDE_DIR}")
        message(FATAL_ERROR "Proton ${PROTON_VERSION} headers were not found in ${PROTON_INCLUDE_DIR}")
    endif (EXISTS "${PROTON_INCLUDE_DIR}")

    if (EXISTS "${PROTON_LIBRARY_DIR}")
        link_directories(${PROTON_LIBRARY_DIR})
    else (EXISTS "${PROTON_LIBRARY_DIR}")
        message(FATAL_ERROR "Proton ${PROTON_VERSION} libraries were not found in ${PROTON_LIBRARY_DIR}")
    endif (EXISTS "${PROTON_LIBRARY_DIR}")
endif (WIN32)

enable_testing()

include_directories(
    src/contrib/
    src/common/
    src/common/exceptions
    src/common/options
)

SET(CPP_RUNTIME_DIR "bin")
SET(CPP_LIBRARY_DIR "lib")
SET(CPP_INCLUDE_DIR "include")

INCLUDE(${CMAKE_SOURCE_DIR}/dist/CMakeInstall.cmake)

if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: None Debug Release."
        FORCE
    )
endif(NOT CMAKE_BUILD_TYPE)

if (CMAKE_COMPILER_IS_GNUCXX)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.7)
        # FIXME: Apparently, the RHEL 7 package is now built with -std=c++11 and
        # there seems to be incompatibility between such library and -std=c++03 built client
        # FIXME: GCC on RHEL 6 does not understand -std=c++03, it's probably building gnu98, anyways...
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")
    endif ()

    set(CMAKE_C_FLAGS "-Wall -Wshadow -fdiagnostics-color=auto" CACHE STRING
        "Flags used by the compiler during all build types." FORCE
    )

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -pipe" CACHE STRING
        "Flags used by the compiler during all build types." FORCE
    )

    set(CMAKE_CXX_FLAGS_DEBUG "-Wall -O2 -g -pipe ${CMAKE_USER_C_FLAGS}" CACHE STRING
        "Flags used by the compiler during all build types." FORCE
    )

    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall -Weffc++ -Wshadow -pipe" CACHE STRING
        "Flags used by the compiler during release." FORCE
    )
endif (CMAKE_COMPILER_IS_GNUCXX)

add_custom_target(distclean
   COMMAND ${CMAKE_BUILD_TOOL} clean
   COMMAND ${CMAKE_COMMAND} -P clean-all.cmake
)

add_custom_target(documentation
   COMMAND ${CMAKE_COMMAND} -P documentation.cmake
)

if (BUILD_WITH_UNIT_TESTS)
    set (CTEST_BINARY_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin)
endif (BUILD_WITH_UNIT_TESTS)

if (ENABLE_MODERN)
    add_definitions(-DENABLE_MODERN)
else (ENABLE_MODERN)
    if (ENABLE_QPID_PROTON)
        message(FATAL_ERROR "Modern code is a requirement for proton code")
    endif(ENABLE_QPID_PROTON)
endif(ENABLE_MODERN)

add_subdirectory(src/contrib)
add_subdirectory(src/common)

if (ENABLE_QPID_PROTON)
    add_definitions(-DENABLE_QPID_PROTON)
    add_subdirectory(src/api/qpid-proton)
endif(ENABLE_QPID_PROTON)

if (ENABLE_QPID_MESSAGING)
    add_definitions(-DENABLE_QPID_MESSAGING)
    add_subdirectory(src/api/qpid)
endif(ENABLE_QPID_MESSAGING)

set(CPPCHECK_EXECUTABLE "/usr/bin/cppcheck" CACHE STRING "Path to cppcheck")
set(CPPCHECK_FLAGS "--report-progress" "-v" "--enable=all" "--xml"
        CACHE STRING "Options for cppcheck")

set(CPPCHECK_DEFS "-DENABLE_MODERN" "-DENABLE_PROTON" "-DENABLE_QPID_MESSAGING"
    "-DQPIDSYSEPOCH" CACHE STRING "Defines for cppcheck")

add_custom_target(check
   COMMAND ${CPPCHECK_EXECUTABLE} ${CPPCHECK_DEFS} ${CPPCHECK_FLAGS} ${CMAKE_BINARY_DIR}
)
