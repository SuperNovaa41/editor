#ifndef DRAW_H
#define DRAW_H

/** BEGIN ASCII ESCAPE STRINGS **/

#define CLEAR_SCREEN_STR "\e[2J"
#define CLEAR_SCREEN_STR_LEN 4

#define MOVE_CURSOR_HOME_STR "\e[H"
#define MOVE_CURSOR_HOME_STR_LEN 3

#define HIDE_CURSOR_STR "\e[?25l"
#define HIDE_CURSOR_STR_LEN 6
#define SHOW_CURSOR_STR "\e[?25h"
#define SHOW_CURSOR_STR_LEN 6

/** END ASCII ESCAPE STRINGS SECTION **/

void refresh_screen(void);

void draw_editor_rows(void);

#endif
