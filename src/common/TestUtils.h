/* 
 * File:   TestUtils.h
 * Author: opiske
 *
 * Created on May 7, 2015, 1:17 PM
 */

#ifndef DTESTS_COMMON_TESTUTILS_H_
#define DTESTS_COMMON_TESTUTILS_H_

#include <cstdio>
#include <cstring>


#define RESET "\x1B[00m"
#define RED "\x1B[00;31m"
#define UNDERLINE "\x1B[04m"

/**
 * Asserts whether the content of a file matches an expected string
 * @param filename the file name to test
 * @param expected the expected string
 * @param size the expected string size
 * @return true if it matches or false otherwise
 */
bool assertFileContent(const char *filename, const char *expected, size_t size);

/**
 * Asserts whether the content of a string matches an expected string
 * @param expected the expected string
 * @param actuals the actual string
 * @return true if it matches or false otherwise
 */
bool assertEquals(const char *expected, const char *actuals);


/**
 * Asserts whether the content of a string matches an expected string
 * @param expected the expected string
 * @param actuals the actual string
 * @param size the expected string size (or the amount of bytes to test for equality)
 * @return true if it matches or false otherwise
 */
bool assertEquals(const char *expected, const char *actuals, size_t size);

#endif /* TESTUTILS_H */

