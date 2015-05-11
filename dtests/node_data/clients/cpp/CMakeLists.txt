PROJECT(dtests-cpp-contrib)

# Ugly hack to avoid newer versions from complaining. Needs to be fixed
cmake_minimum_required(VERSION 2.4)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/lib)




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
 
