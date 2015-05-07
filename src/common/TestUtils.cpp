#include "TestUtils.h"


bool assertFileContent(const char *filename, const char *expected, size_t size) { 
	char buffer[256] = { 0 };
	FILE *readStream = fopen(filename, "r"); 
	
	if (!readStream) {
		fprintf(stderr, "Unable to open output file %s for reading", 
			filename);
		goto error;
	}
	
	fread(buffer, size + 1, 1, readStream);
	
	if (strncmp(expected, buffer, size) != 0) {
		fprintf(stderr, "The strings are different: %s != %s", 
			expected, buffer);
		goto error;
	}
	
	fclose(readStream);
	return true;
	
	error:	
	fclose(readStream);
	return false;
}