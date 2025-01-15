#include <unistd.h>

#include "headers/controls.h"
#include "headers/draw.h"
#include "headers/term.h"

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
		else
			parse_input_char(c);
		refresh_screen();
	}

	return EXIT_SUCCESS;
}
