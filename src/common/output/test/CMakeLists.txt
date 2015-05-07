PROJECT(dtests-proton-cpp-tests)
   
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/tests/bin)

cmake_policy(SET CMP0003 OLD)
add_executable(ut_printer_test
	ut_printer_test.cpp
)

target_link_libraries(ut_printer_test
           dtests-cpp-common)

add_test(NAME PrinterTest COMMAND ut_printer_test)