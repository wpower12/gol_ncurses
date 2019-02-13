#ifndef GOL_NCURSES_H
#define GOL_NCURSES_H

void random_world(int**, int**, int, int);
void update_world(int**, int**, int, int);
void render_world(int**, int, int);

#define EMPTY_CELL 0
#define LIVE_CELL  1

#endif