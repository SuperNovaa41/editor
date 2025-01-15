#include <unistd.h>

#include "draw.h"
#include "term.h"

/** BEGIN extern section **/

extern editor_t editor;
extern screen_buffer_t screen_buffer;

/** END extern section **/

void refresh_screen(void)
{
	draw_editor_rows();

	write(STDOUT_FILENO, screen_buffer.text, screen_buffer.len);			
}

void draw_editor_rows(void)
{
	size_t i;
	for (i = 0; i < editor.rows - 1; i++)
		screen_buffer_append(&screen_buffer, "~\r\n", 3);
}
