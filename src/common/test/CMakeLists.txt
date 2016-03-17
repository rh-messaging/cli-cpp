PROJECT(dtests-proton-cpp-tests)
   
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/tests/bin)

cmake_policy(SET CMP0003 OLD)
add_executable(ut_beanutils_test
	ut_beanutils_test.cpp
	FakeBean.cpp
)

target_link_libraries(ut_beanutils_test
           dtests-cpp-common)

add_test(NAME BeanUtilsTest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
    COMMAND ut_beanutils_test)


cmake_policy(SET CMP0003 OLD)
add_executable(ut_stlutils_test
	ut_stlutils_test.cpp
)

target_link_libraries(ut_stlutils_test
           dtests-cpp-common)

add_test(NAME STLUtilsTest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
    COMMAND ut_stlutils_test)