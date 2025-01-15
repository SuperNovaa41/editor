#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "draw.h"
#include "term.h"

#define MAX_LEN 256

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
	char* move_cursor;

	cur_x = editor.cx;
	cur_y = editor.cy;

	// hide the cursor to prevent flickering
	screen_buffer_append(&screen_buffer, HIDE_CURSOR_STR, HIDE_CURSOR_STR_LEN);

	screen_buffer_append(&screen_buffer, MOVE_CURSOR_HOME_STR, MOVE_CURSOR_HOME_STR_LEN);

	screen_buffer_append(&screen_buffer, CLEAR_SCREEN_STR, CLEAR_SCREEN_STR_LEN);

	draw_editor_rows();

	move_cursor_pos(&move_cursor, cur_x, cur_y);
	screen_buffer_append(&screen_buffer, move_cursor, strlen(move_cursor));
	free(move_cursor);

	screen_buffer_append(&screen_buffer, SHOW_CURSOR_STR, SHOW_CURSOR_STR_LEN);

	write(STDOUT_FILENO, screen_buffer.text, screen_buffer.len);			
}

void draw_editor_rows(void)
{
	size_t i;
	for (i = 0; i < editor.rows - 1; i++)
		screen_buffer_append(&screen_buffer, "~\r\n", 3);
}


void move_cursor_pos(char** out, int x, int y)
{
	*out = malloc(sizeof(char) * MAX_LEN);
	snprintf(*out, MAX_LEN, "\e[%d;%dH", x, y);
}

#undef MAX_LEN
