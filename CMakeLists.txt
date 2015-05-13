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
    message("64 bits compiler detected" )
    set(DTEST_BUILD_PLATFORM 64)
    set(DTEST_BUILD_PLATFORM_NAME "x64" )
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    MESSAGE( "32 bits compiler detected" )
    set(DTEST_BUILD_PLATFORM 32)
    set(DTEST_BUILD_PLATFORM_NAME "x84" )
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

find_package(Boost 1.36.0)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else(Boost_FOUND)
    set(BOOST_VERSION 141) 
    set(BOOST_NON_DEFAULT_INCLUDE_DIR "/usr/include/boost${BOOST_VERSION}/")
    set(BOOST_NON_DEFAULT_LIBRARY_DIR "/usr/lib/boost${BOOST_VERSION}/")

    if (UNIX)
	if (EXISTS "${BOOST_NON_DEFAULT_INCLUDE_DIR}")
	        include_directories(${BOOST_NON_DEFAULT_INCLUDE_DIR})
	else (EXISTS "${BOOST_NON_DEFAULT_INCLUDE_DIR}") 
		message(FATAL_ERROR "Boost ${BOOST_VERSION} headers were not found in ${BOOST_NON_DEFAULT_INCLUDE_DIR}")
	endif (EXISTS "${BOOST_NON_DEFAULT_INCLUDE_DIR}") 

	if (EXISTS "${BOOST_NON_DEFAULT_LIBRARY_DIR}")
	       	link_directories(${BOOST_NON_DEFAULT_LIBRARY_DIR})
	else (EXISTS "${BOOST_NON_DEFAULT_LIBRARY_DIR}") 
		message(FATAL_ERROR "Boost ${BOOST_VERSION} libraries were not found")
	endif (EXISTS "${BOOST_NON_DEFAULT_LIBRARY_DIR}") 

    endif(UNIX)
endif(Boost_FOUND)

enable_testing()

include_directories(
	src/contrib/
	src/common/
	src/common/exceptions
	src/common/options
)


if (CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS "-Wall -Wshadow " CACHE STRING
		"Flags used by the compiler during all build types." FORCE
	)

	set(CMAKE_CXX_FLAGS "-Wall -O2 -g -pipe" CACHE STRING
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

set(ENABLE_PROTON OFF CACHE BOOL "Enable proton clients")
set(ENABLE_PROTON_REACTOR OFF CACHE BOOL "Enable proton reactor clients")
set(ENABLE_QPID ON CACHE BOOL "Enable qpid clients")
set(ENABLE_MODERN OFF CACHE BOOL "Enable usage of refactored code")
set(BUILD_WITH_UNIT_TESTS OFF CACHE BOOL "Build unit tests for the CPP client code")

if (BUILD_WITH_UNIT_TESTS)
    set (CTEST_BINARY_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin)
endif (BUILD_WITH_UNIT_TESTS)

if (ENABLE_MODERN) 
    add_definitions(-DENABLE_MODERN) 
endif(ENABLE_MODERN)

add_subdirectory(src/contrib)
add_subdirectory(src/common)

if (ENABLE_PROTON)
    add_definitions(-DENABLE_PROTON) 
    add_subdirectory(src/api/proton)
endif(ENABLE_PROTON)


if (ENABLE_QPID) 
    add_definitions(-DENABLE_QPID)    
    add_subdirectory(src/api/qpid)
endif(ENABLE_QPID)
 
