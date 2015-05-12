/* 
 * File:   TestUtils.h
 * Author: opiske
 *
 * Created on May 7, 2015, 1:17 PM
 */

#ifndef TESTUTILS_H
#define	TESTUTILS_H

#include <cstdio>
#include <cstring>


#define RESET "\x1B[00m"
#define RED "\x1B[00;31m"
#define UNDERLINE "\x1B[04m"

bool assertFileContent(const char *filename, const char *expected, size_t size);

bool assertEquals(const char *expected, const char *actuals);
bool assertEquals(const char *expected, const char *actuals, size_t size);

#endif	/* TESTUTILS_H */

