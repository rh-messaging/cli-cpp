PROJECT(dtests-proton-cpp-tests)
   
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/tests/bin)

cmake_policy(SET CMP0003 OLD)
add_executable(ut_logger_test
	ut_logger_test.cpp
)

target_link_libraries(ut_logger_test
           dtests-cpp-common)

add_test(NAME LoggerTest 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
    COMMAND ut_logger_test)


cmake_policy(SET CMP0003 OLD)
add_executable(ut_logger_test_stdout
	ut_logger_test_stdout.cpp
)

target_link_libraries(ut_logger_test_stdout
           dtests-cpp-common)

add_test(NAME LoggerTestStdOut 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
    COMMAND ut_logger_test_stdout)