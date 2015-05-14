/* 
 * File:   PrinterTest.cpp
 * Author: opiske
 *
 * Created on May 7, 2015, 10:42 AM
 */

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "output/Printer.h"
#include "TestUtils.h"

static void createTestFile(const char *testFileName, const char *writeString) {
	
	FILE *writeStream = fopen(testFileName, "w+");
	
	if (!writeStream) {
		fprintf(stderr, "Unable to open output file for writing");
		exit(1);
	}

	Printer printer = Printer(writeStream);
	printer.print(writeString);
	fclose(writeStream);
}

/*
 * 
 */
int main(int argc, char** argv) {
	char testFileName[] = "printer-test.log";
	char writeString[] = "This is a test";
	
	createTestFile(testFileName, writeString);
	
	size_t len = strlen(writeString);
	bool ret = assertFileContent(testFileName, writeString, len);
	if (!ret) {
		return 1;
	}
	
	return 0;
}

