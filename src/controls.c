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

void move_cursor(movement_t dir)
{
	switch(dir) {
		case LEFT:
			move_cursor_pos(--editor.cx, editor.cy);	
			break;
		case RIGHT:
			move_cursor_pos(++editor.cx, editor.cy);
			break;
		case UP:
			move_cursor_pos(editor.cx, --editor.cy);
			break;
		case DOWN:
			move_cursor_pos(editor.cx, ++editor.cy);
			break;
		case HOME:
			move_cursor_pos(1, 1);
			editor.cx = 1;
			editor.cy = 1;
			break;
		case BEGIN_LINE:
			move_cursor_pos(1, editor.cy);
			editor.cx = 1;
			break;
		default:
			fprintf(stderr, "move_cursor had an invalid input, what the hell.\n");
			exit(EXIT_FAILURE);
	}
}

