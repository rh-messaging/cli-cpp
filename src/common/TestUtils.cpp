#include <stdlib.h>

#include "TestUtils.h"


bool assertFileContent(const char *filename, const char *expected, size_t size) { 
	FILE *readStream = NULL;
	char *buffer = NULL;
	bool ret = false;
	
	buffer = (char *) malloc(size + 1); 
	if (!buffer) {
		fprintf(stderr, "Unable to allocate %i bytes for memory buffer",
			size);
		goto err_sys;
	}
	bzero(buffer, size + 1);
	
	readStream = fopen(filename, "r"); 
	
	if (!readStream) {
		fprintf(stderr, "Unable to open output file %s for reading", 
			filename);
		goto err_test;
	}
	
	fread(buffer, size, 1, readStream);
	
	if (!assertEquals(expected, buffer, size)) {
		goto err_test;
	}
	
	ret = true;
	
	err_test:
	fclose(readStream);
	
	err_sys:
	free(buffer);

	return ret;
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