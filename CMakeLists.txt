PROJECT(dtests-cpp-contrib)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.4)

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


if( CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "64 bits compiler detected")
    set(DTEST_BUILD_PLATFORM 64)
    set(DTEST_BUILD_PLATFORM_NAME "x64")
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "32 bits compiler detected")
    set(DTEST_BUILD_PLATFORM 32)
    set(DTEST_BUILD_PLATFORM_NAME "x84")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

set(BOOST_INCLUDE_DIR "/usr/include" CACHE String "Boost include directory")
set(BOOST_LIBRARY_DIR "/usr/lib" CACHE String "Boost library directory")


if (EXISTS "${BOOST_INCLUDE_DIR}")
        include_directories(${BOOST_INCLUDE_DIR})
else (EXISTS "${BOOST_INCLUDE_DIR}") 
	message(FATAL_ERROR "Boost ${BOOST_VERSION} headers were not found in ${BOOST_INCLUDE_DIR}")
endif (EXISTS "${BOOST_INCLUDE_DIR}") 

if (EXISTS "${BOOST_LIBRARY_DIR}")
       	link_directories(${BOOST_LIBRARY_DIR})
else (EXISTS "${BOOST_LIBRARY_DIR}") 
	message(FATAL_ERROR "Boost ${BOOST_VERSION} libraries were not found in ${BOOST_LIBRARY_DIR}")
endif (EXISTS "${BOOST_LIBRARY_DIR}") 


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

INCLUDE(${CMAKE_BINARY_DIR}/dist/CMakeInstall.cmake)

if(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug CACHE STRING
      	"Choose the type of build, options are: None Debug Release."
      	FORCE
	)
endif(NOT CMAKE_BUILD_TYPE)
 
if(NOT DISABLE_IMPLICIT_CONVERSIONS)
    add_definitions(-DENABLE_IMPLICIT_CONVERSIONS)
else(NOT DISABLE_IMPLICIT_CONVERSIONS)
    message(STATUS "Implicit conversions are disabled")
endif(NOT DISABLE_IMPLICIT_CONVERSIONS)

if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS "-Wall -Wshadow -fdiagnostics-color=auto" CACHE STRING
		"Flags used by the compiler during all build types." FORCE
	)

	set(CMAKE_CXX_FLAGS "-Wall -O2 -pipe" CACHE STRING
		"Flags used by the compiler during all build types." FORCE
	)

        set(CMAKE_CXX_FLAGS_DEBUG "-Wall -O2 -g -pipe -std=c++03 -fdiagnostics-color=auto" CACHE STRING
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

set(ENABLE_QPID_PROTON OFF CACHE BOOL "Enable qpid proton clients")
set(ENABLE_QPID_MESSAGING ON CACHE BOOL "Enable qpid messaging clients")
set(ENABLE_MODERN OFF CACHE BOOL "Enable usage of refactored code")
set(BUILD_WITH_UNIT_TESTS OFF CACHE BOOL "Build unit tests for the CPP client code")

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
