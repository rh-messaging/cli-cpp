#include <stdlib.h>

#include "TestUtils.h"


bool assertFileContent(const char *filename, const char *expected, size_t size) { 
	char buffer[size + 1] = { 0 };
	FILE *readStream = fopen(filename, "r"); 
	
	if (!readStream) {
		fprintf(stderr, "Unable to open output file %s for reading", 
			filename);
		goto error;
	}
	
	fread(buffer, size, 1, readStream);
	
	if (!assertEquals(expected, buffer, size)) {
		goto error;
	}
	
	fclose(readStream);
	return true;
	
	error:	
	fclose(readStream);
	return false;
}

bool assertEquals(const char *expected, const char *actual) {
	return assertEquals(expected, actual, strlen(expected));
}

bool assertEquals(const char *expected, const char *actual, size_t size) { 
	if (strncmp(expected, actual, size) != 0) {
		fprintf(stderr, "The strings are different\nE: %s\nA: ", 
			expected);
		
		for (size_t i = 0; i < size; i++) {
			if (expected[i] == actual[i]) {
				fprintf(stderr, "%c", actual[i]);
			}
			else {
				fprintf(stderr, "%s%c", UNDERLINE,
					actual[i]);
			}
			
		}
		
		
		fprintf(stderr, "%s\n", RESET);
		return false;
	}
	
	fprintf(stderr, "%s\n", RESET);
	return true;
}