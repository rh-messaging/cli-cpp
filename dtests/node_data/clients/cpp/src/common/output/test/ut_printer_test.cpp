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

/*
 * 
 */
int main(int argc, char** argv) {
	char writeString[] = "This is a test";
	char testFileName[] = "printer-test.log";
	
	FILE *writeStream = fopen(testFileName, "w+");
	
	if (!writeStream) {
		fprintf(stderr, "Unable to open output file for writing");
		return 1;
	}
	
	Printer printer = Printer(writeStream);
	
	printer.print(writeString);
	
	char buffer[256] = { 0 };
	
	FILE *readStream = fopen(testFileName, "r+"); 
	if (!readStream) {
		fprintf(stderr, "Unable to open output file for reading");
		return 1;
	}
	
	fread(buffer, sizeof(buffer), 1, readStream);
	
	if (strncmp(writeString, buffer, strlen(buffer)) != 0) {
		fprintf(stderr, "The strings are different: %s != %s", 
			writeString, buffer);
		return 2;
	}
	
	return 0;
}

