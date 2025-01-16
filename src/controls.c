#include <stdio.h>

#include "include/__defines.h"
#include "include/controls.h"
#include "include/draw.h"
#include "include/term.h"

extern editor_t editor;
extern screen_buffer_t screen_buffer;

int parse_input_char(char input_char)
{
	if (INSERT_MODE == editor.control_type) {
		if (ESC == input_char) {
			editor.control_type = COMMAND_MODE;
			return 0;
		}

		// put char
		
		return 0;
	}

	switch(input_char) {
		/** INSERT MODE KEYS START **/
		case 'a':
			move_cursor(RIGHT);
		case 'i':
			editor.control_type = INSERT_MODE;
			break;
		/** INSERT MODE KEYS END **/

		/** MOVEMENT KEYS START **/

		case 'h':
			move_cursor(LEFT);
			break;
		case 'j':
			move_cursor(DOWN);
			break;
		case 'k':
			move_cursor(UP);
			break;
		case 'l':
			move_cursor(RIGHT);
			break;

		case 'g': // TODO: move this to 'gg'
			move_cursor(HOME);
			break;
		case 'G':
			// want to move to the end of the buffer
			break;

		case '0':
			move_cursor(BEGIN_LINE);
			break;


		/** MOVEMENT KEYS END **/
		default:
			return -1;
	}
	return 0;
}

void do_falldown(row_t* row)
{
	if (editor.cx > row->len) {
		editor.cx = row->len;
		editor.rx = row->len;
		move_cursor_pos(editor.cx, editor.cy);
	}
}

/**
 * TODO: for some reason the cursor is no longer respectingl ine length
 * when checking where to move
 *
 * i imagine this might be something to do with the new functions i added to setup
 * rows, and that len isn't properly being translated
 *
 * check this, if i can't figure it out maybe try valgrind to track everything
 */

void move_cursor(movement_t dir)
{
	switch(dir) {
		case LEFT:
			if (editor.cx - 1 <= 0)
				break;
			move_cursor_pos(--editor.cx, editor.cy);	
			editor.rx--;
			break;
		case RIGHT:
			if (editor.cx + 1 > editor.rows[editor.ry - 1].len 
					&& editor.cx + 1 > editor.screen_cols)
				break;
			move_cursor_pos(++editor.cx, editor.cy);
			editor.rx++;
			break;
		case UP:
			if (editor.cy - 1 <= 0) {
				if (editor.row_offset == 0)
					break;
				editor.row_offset--;
				editor.ry--;
				break;
			}
			move_cursor_pos(editor.cx, --editor.cy);
			editor.ry--;
			
			// want to fall down to the end of the line if we're moving to a shorter one
			do_falldown(&editor.rows[editor.ry - 1]);

			break;
		case DOWN:
			if (editor.cy + 1 >= editor.screen_rows - 1) {
				if (editor.ry + 1 >= editor.num_rows)
					break;
				editor.row_offset++;
				editor.ry++;
				break;
			}

			move_cursor_pos(editor.cx, ++editor.cy);
			editor.ry++;

			do_falldown(&editor.rows[editor.ry - 1]);

			break;
		case HOME:
			move_cursor_pos(1, 1);
			editor.cx = 1; editor.cy = 1;
			editor.rx = 1; editor.ry = 1;
			break;
		case BEGIN_LINE:
			move_cursor_pos(1, editor.cy);
			editor.cx = 1;
			editor.rx = 1;
			break;
		default:
			fprintf(stderr, "move_cursor had an invalid input, what the hell.\n");
			exit(EXIT_FAILURE);
	}
}

#undef DO_FALLDOWN
