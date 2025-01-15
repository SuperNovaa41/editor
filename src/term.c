#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>


#include "headers/term.h"

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

	editor.cx = 1;
	editor.cy = 1;

	editor.cols = win.ws_col;
	editor.rows = win.ws_row;
}

void kill_application(void)
{
	/**
	 * TODO: implement more things that need to be cleaned up when
	 * killing the application
	 */
	reset_input_mode();

	screen_buffer_free(&screen_buffer);
}


int screen_buffer_append(screen_buffer_t *buf, const char *in, size_t len)
{
	// Need to realloc to allow for the input to fit into the buffer
	char* new = realloc(buf->text, buf->len + len);

	if (NULL == new) return -1;

	memcpy(&new[buf->len], in, len);
	buf->text = new;
	buf->len += len;
	
	return 0;
}

void screen_buffer_free(screen_buffer_t *buf)
{
	free(buf->text);
}
