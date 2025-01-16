#include <stdlib.h>
#include <termios.h>

#ifndef TERM_H
#define TERM_H

typedef enum {
	COMMAND_MODE,
	INSERT_MODE,
} control_type_t;

typedef struct {
	char* line;
	size_t len;

	char* render;
	size_t r_len;
} row_t;

typedef struct {
	struct termios term_settings;
	control_type_t control_type;
	
	size_t row_offset;
	row_t* rows;
	size_t num_rows;

	int rx, ry;

	size_t screen_cols, screen_rows;
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
int screen_buffer_append(const char* in, size_t len);

/**
 * # screen_buffer_free
 *
 * - screen_buffer_t* buf
 *
 * Frees the screen buffer
 *
 */
void screen_buffer_free(screen_buffer_t* buf);

/**
 * # free_editor_row
 *
 * Frees all of the strings in each row
 * and the row array
 */
void free_editor_row(void);

void editor_render_row(row_t* row);

void editor_add_row(const char* line, size_t len);

#endif
