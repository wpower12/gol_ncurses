all: 
	gcc src/gol_ncurses.c -l ncurses -o bin/gol

chw:
	gcc src/curses_hw.c -l ncurses -o bin/chw