#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "headers/draw.h"
#include "headers/term.h"

#define MAX_LEN 256

/** BEGIN extern section **/

extern editor_t editor;
extern screen_buffer_t screen_buffer;

/** END extern section **/

void refresh_screen(void)
{
	/**
	 * TODO: 
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
	draw_cursor_pos();

	move_cursor_pos(cur_x, cur_y);

	screen_buffer_append(&screen_buffer, SHOW_CURSOR_STR, SHOW_CURSOR_STR_LEN);

	write(STDOUT_FILENO, screen_buffer.text, screen_buffer.len);			
}

void draw_editor_rows(void)
{
	size_t i;
	for (i = 0; i < editor.rows - 1; i++)
		screen_buffer_append(&screen_buffer, "~\r\n", 3);
}

void draw_cursor_pos(void)
{
	char* pos = malloc(sizeof(char) * MAX_LEN);
	snprintf(pos, MAX_LEN, "%d,%d", editor.cx, editor.cy);
	screen_buffer_append(&screen_buffer, pos, strlen(pos));
	free(pos);
}

void move_cursor_pos(int x, int y)
{
	// TODO: add check for boundaries
	char* out = malloc(sizeof(char) * MAX_LEN);
	snprintf(out, MAX_LEN, "\e[%d;%dH", y, x);
	screen_buffer_append(&screen_buffer, out, strlen(out));
	free(out);
}

#undef MAX_LEN
