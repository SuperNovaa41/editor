#include "term.h"

#ifndef CONTROLS_H
#define CONTROLS_H

typedef enum {
	UP, DOWN,
	LEFT, RIGHT,
	HOME,
	BEGIN_LINE,
} movement_t;

int parse_input_char(char in);

void do_falldown(row_t* row);

void move_cursor(movement_t dir);

#endif
