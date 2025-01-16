#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>


#include "include/term.h"

/** BEGIN extern section **/

extern editor_t editor;
extern screen_buffer_t screen_buffer;

/** END extern section **/

void reset_input_mode(void)
{
	int err;
	/**
	 * STDIN_FILENO - the file descriptor for STDIN, provuded by unistd
	 * TCSANOW - means to enact the change NOW, provided by termios
	 */
	err = tcsetattr(STDIN_FILENO, TCSANOW, &(editor.term_settings));
	if (-1 == err) {
		fprintf(stderr, "tcsetattr failed.\n");
		exit(EXIT_FAILURE);
	}
}

void save_input_mode(void)
{
	int err;

	err = tcgetattr(STDIN_FILENO, &(editor.term_settings));
	if (-1 == err) {
		fprintf(stderr, "tcgetattr failed.\n");
		exit(EXIT_FAILURE);
	}
	atexit(kill_application);
}

void set_input_mode(void)
{
	struct termios tattr;
	int err;

	// make sure we're actually in a terminal
	if (!isatty(STDIN_FILENO)) {
		fprintf(stderr, "Not a terminal.\n");
		exit(EXIT_FAILURE);
	}

	// need to save the terminal settings so that
	// we can restore them on kill
	save_input_mode();

	err = tcgetattr(STDIN_FILENO, &tattr);
	if (-1 == err) {
		fprintf(stderr, "tcgetattr failed.\n");
		exit(EXIT_FAILURE);
	}
	tattr.c_lflag &= ~(ICANON|ECHO); // clears icanon and echo flags
	
	/**
	 * This is the minimum number of bytes in the input
	 * queue in order for read to return.
	 * We need this as one because it will be constantly looking for one char input.
	 *
	 * Zero would be consistently reading null,
	 * any higher would bel ooking for a specific amount
	 * of characters each time it tries to read.
	 */
	tattr.c_cc[VMIN] = 1;

	/**
	 * This is specifying how long we want to wait for input
	 * before returning.
	 * We want this to be set to zero so that
	 * we are constantly reading.
	 */
	tattr.c_cc[VTIME] = 0;

	/**
	 * TCSAFLUSH waits for all of the output in the buffer
	 * to be outputted before we discard all of the settings
	 * and reset the terminal mode
	 */
	err = tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
	if (-1 == err) {
		fprintf(stderr, "tcsetattr failed.\n");
		exit(EXIT_FAILURE);
	}
}

void setup_terminal(void)
{
	struct winsize win;
	int err;

	set_input_mode();

	err = ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
	if (-1 == err) {
		fprintf(stderr, "ioctl w/ TIOCGWINSZ failed to fetch screen size.\n");
		exit(EXIT_FAILURE);
	}

	editor.control_type = COMMAND_MODE;

	editor.num_rows = 0;
	editor.rows = NULL;
	editor.row_offset = 0;

	editor.rx = 1;
	editor.ry = 1;

	editor.cx = 1;
	editor.cy = 1;

	editor.screen_cols = win.ws_col;
	editor.screen_rows = win.ws_row;
}

void kill_application(void)
{
	/**
	 * TODO: implement more things that need to be cleaned up when
	 * killing the application
	 */
	reset_input_mode();

	screen_buffer_free(&screen_buffer);

	free_editor_row();
}


int screen_buffer_append(const char *in, size_t len)
{
	// Need to realloc to allow for the input to fit into the buffer
	char* new = realloc(screen_buffer.text, screen_buffer.len + len);

	if (NULL == new) return -1;

	memcpy(&new[screen_buffer.len], in, len);
	screen_buffer.text = new;
	screen_buffer.len += len;
	
	return 0;
}

void screen_buffer_free(screen_buffer_t *buf)
{
	free(buf->text);
}

void free_editor_row(void)
{
	size_t i;
	for (i = 0; i < editor.num_rows; i++)
		free(editor.rows[i].line);
	free(editor.rows);
}

void editor_render_row(row_t* row)
{
	if (row->render != NULL) // if the row is malloc'd we want to delete it
		free(row->render);	
	row->render = malloc(sizeof(char) * row->len);
}

// small helper function to initialize a row
void create_row(row_t* row, size_t len)
{
	row->len = len;
	row->line = malloc(sizeof(char) * (len + 1));

	row->render = NULL;
	row->r_len = 0;
}

void editor_add_row(const char* line, size_t len)
{
	// Need to add space for another row
	editor.rows = realloc(editor.rows, sizeof(row_t) * (editor.num_rows + 1));

	// Set len and malloc space for the incoming line +1 for \0
	size_t idx = editor.num_rows;
	
	create_row(&editor.rows[idx], len);

	memcpy(editor.rows[idx].line, line, len); 
	editor.rows[idx].line[len] = '\0'; // need to null terminate it

	editor.num_rows++; 
}
