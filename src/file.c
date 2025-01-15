#include <stdlib.h>
#include <stdio.h>

#include "include/file.h"
#include "include/term.h"

#define MAX_LINE_LEN 1024

void read_file(const char* filename)
{
	ssize_t linelen;
	size_t n;
	char* line = NULL;	

	FILE* fp = fopen(filename, "r");
	if (NULL == fp) {
		fprintf(stderr, "failed to open file for reading.\n");
		exit(EXIT_FAILURE);
	}
	
	while (-1 != (linelen = getline(&line, &n, fp))) {
		editor_add_row(line, linelen);
	}

	fclose(fp);
}
