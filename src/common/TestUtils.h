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

bool assertFileContent(const char *filename, const char *expected, size_t size);

#endif	/* TESTUTILS_H */

