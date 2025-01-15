#include <stdlib.h>
#include <termios.h>

#ifndef TERM_H
#define TERM_H

typedef struct {
	struct termios term_settings;
	size_t cols, rows;
	int cx, cy; // cursor pos
} editor_t;

typedef struct {
	char* text;
	size_t len;
} screen_buffer_t;

/**
 * # reset_input_mode
 *
 * Resets the term settings back to their original configuration
 *
 */
void reset_input_mode(void);

/**
 * # save_input_mode
 *
 * Saves the current terminal settings, so that they can be restored
 *
 */
void save_input_mode(void);

/**
 * # set_input_mode
 *
 * Sets the current terminal settings to enable non-canonical mode
 * so we can fully control the screen to employ the editor
 *
 */
void set_input_mode(void);

/**
 * # setup_terminal
 *
 * Sets the variables and initializes various things to make
 * the editor usable
 *
 */
void setup_terminal(void);

/**
 * # kill_application
 *
 * Frees, resets, etc the things we need to do when closing the
 * application so everything returns to normal after closing
 *
 */
void kill_application(void);

/**
 * # screen_buffer_append
 *
 * - screen_buffer_t* buf : The screen buffer
 * - const char* in : The input text
 * - size_t len : Length of the input text
 *
 * Adds to the screen buffer
 *
 * ## RETURN VALUE
 *
 * Returns -1 on failure, 0 on success
 *
 */
int screen_buffer_append(screen_buffer_t* buf, const char* in, size_t len);

/**
 * # screen_buffer_free
 *
 * - screen_buffer_t* buf
 *
 * Frees the screen buffer
 *
 */
void screen_buffer_free(screen_buffer_t* buf);

#endif
