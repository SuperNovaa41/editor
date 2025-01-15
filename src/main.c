#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/controls.h"
#include "include/draw.h"
#include "include/file.h"
#include "include/term.h"

editor_t editor;
screen_buffer_t screen_buffer;

int main(int argc, char** argv)
{
	char c;
	char* in;

	if (argc > 2) {
		fprintf(stderr, "Usage: %s <filename>\n || Usage: %s", argv[0], argv[0]);
		return EXIT_FAILURE;
	} 

	setup_terminal();

	if (argc == 2)
		read_file(argv[1]);

	refresh_screen(); // want to draw first since we're going to be waiting on the read

	while (1) {
		read(STDIN_FILENO, &c, 1);
		if (c == '\004')
			break;
		else
			parse_input_char(c);
		refresh_screen();
	}

	return EXIT_SUCCESS;
}
