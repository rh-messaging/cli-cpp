PROJECT(dtests-messaging-cpp-tests)
   
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/tests/bin)

cmake_policy(SET CMP0003 OLD)
add_executable(ut_qpid_decoder_test
	ut_qpid_decoder_test.cpp
)

target_link_libraries(ut_qpid_decoder_test
           dtests-qpid-messaging-common)

add_test(NAME QpidDecoderTest 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
    COMMAND ut_qpid_decoder_test)