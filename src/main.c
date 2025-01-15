#include <stdio.h>
#include <unistd.h>

#include "draw.h"
#include "term.h"

editor_t editor;
screen_buffer_t screen_buffer;

int main(void)
{
	char c;
	char* in;

	setup_terminal();

	refresh_screen(); // want to draw first since we're going to be waiting on the read

	while (1) {
		read(STDIN_FILENO, &c, 1);
		if (c == '\004')
			break;
		refresh_screen();
	}

	return EXIT_SUCCESS;
}
