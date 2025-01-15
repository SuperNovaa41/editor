#include <unistd.h>

#include "draw.h"
#include "term.h"

/** BEGIN extern section **/

extern editor_t editor;
extern screen_buffer_t screen_buffer;

/** END extern section **/

void refresh_screen(void)
{
	/**
	 * TODO: need to move the cursor back at the end
	 * also need to draw the cusor coords at the bottom
	 * and need to implement controls
	 */

	int cur_x, cur_y;

	cur_x = editor.cx;
	cur_y = editor.cy;

	// hide the cursor to prevent flickering
	screen_buffer_append(&screen_buffer, HIDE_CURSOR_STR, HIDE_CURSOR_STR_LEN);

	screen_buffer_append(&screen_buffer, MOVE_CURSOR_HOME_STR, MOVE_CURSOR_HOME_STR_LEN);

	screen_buffer_append(&screen_buffer, CLEAR_SCREEN_STR, CLEAR_SCREEN_STR_LEN);

	draw_editor_rows();

	screen_buffer_append(&screen_buffer, SHOW_CURSOR_STR, SHOW_CURSOR_STR_LEN);

	write(STDOUT_FILENO, screen_buffer.text, screen_buffer.len);			
}

void draw_editor_rows(void)
{
	size_t i;
	for (i = 0; i < editor.rows - 1; i++)
		screen_buffer_append(&screen_buffer, "~\r\n", 3);
}
