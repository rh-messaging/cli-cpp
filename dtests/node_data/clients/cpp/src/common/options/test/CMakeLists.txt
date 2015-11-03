PROJECT(dtests-proton-cpp-options-tests)
   
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/tests/bin)

cmake_policy(SET CMP0003 OLD)
add_executable(ut_options_test
	ut_options_test.cpp
)

target_link_libraries(ut_options_test
           dtests-cpp-common
	   dtests-cpp-contrib)

add_test(NAME MapOptionsTest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
    COMMAND ut_options_test --map "country1:Bahamas;country2:Czech Republic"
)