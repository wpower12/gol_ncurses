#include <ncurses.h> // Used for all the graphics! 
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "gol_ncurses.h"

#define MAX_RANDVAL   100
#define THRES_RANDVAL 15
#define DISPLAY_CHAR  ACS_PLUS

int mod(int, int);
void delay(int milliseconds);

int main()
{	
	// Init
	int rows, cols;
	srand(time(NULL));

	initscr(); // Start curses mode 
	getmaxyx(stdscr, rows, cols);

	// For debugging - Getting weird periods
	// printw("termsize: %i x %i", cols, rows);
	// getch();
	
	nodelay(stdscr, true);

	// Make the world
	int **world, **buff;
   	world = (int **) malloc(cols * sizeof(int*));
   	buff  = (int **) malloc(cols * sizeof(int*));
   	for(int j=0; j<cols;j++)
   	{
   		world[j] = (int *) malloc(rows * sizeof(int));
   		buff[j]  = (int *) malloc(rows * sizeof(int));
   	}
	random_world(world, buff, cols, rows);

	// Simulation Loop
	bool running = true;
	while(running)
	{	
		erase();
		render_world(world, cols, rows);
		refresh();
		update_world(world, buff, cols, rows);
		// The following gets us to quit on any keypress
		if(getch() != ERR){ running = false; }
		delay(100);
	}
	endwin();			/* End curses mode		  		  */
	return 0;
}

void random_world(int** w, int** b, int cols, int rows)
{
	for(int i=0;i<cols;i++)
	{
		for(int j=0;j<rows;j++)
		{
			if((rand()%MAX_RANDVAL) < THRES_RANDVAL)
			{
				w[j][i] = LIVE_CELL;
				b[j][i] = LIVE_CELL;
			} else {
				w[j][i] = EMPTY_CELL;
				b[j][i] = EMPTY_CELL;
			}
		}
	}
}

void render_world(int** w, int cols, int rows)
{
	for(int i=0;i<cols;i++)
	{
		for(int j=0;j<rows;j++)
		{
			if(w[j][i] == LIVE_CELL)
			{
				mvaddch(j,i,DISPLAY_CHAR);
			} 
		}
	}
}

void update_world(int** w, int** b, int cols, int rows)
{	
	// w - world which will hold the 'result'.
	// b - buffer/copy of world used to read neighbors from.
	int num_live, num_dead, cell_value;
	for(int i=0;i<cols;i++)
	{
		for(int j=0;j<rows;j++)
		{
			// read neighbor counts from b
			num_live = 0;
			num_dead = 0;
			for(int n_x=-1;n_x<2;n_x++)
			{
				for(int n_y=-1;n_y<2;n_y++)
				{	
					// We never count the middle cell 
					if(!(n_x == 0 && n_y == 0))
					{
						cell_value = b[mod(j+n_y,rows)][mod(i+n_x,cols)];
						if(cell_value == LIVE_CELL) {num_live++;}
						if(cell_value == EMPTY_CELL){num_dead++;}
					}
				}
			}
			// apply the GoL rules, writing result to w NOT b.
			cell_value = b[j][i];
			if(cell_value == LIVE_CELL)
			{
				if(num_live > 3 || num_live < 2)
				{
					w[j][i] = EMPTY_CELL;
				}
			} else {
				if(num_live == 3){
					w[j][i] = LIVE_CELL;
				}
			}	
		}
	}

	// deep copy the world back into the buffer.
	for(int i=0;i<cols;i++)
	{
		for(int j=0;j<rows;j++)
		{
			b[j][i] = w[j][i];
		}
	}
}

int mod(int a, int b){
	if(a < 0)
	{
		return b+a;
	} else {
		return a%b;
	}
}

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;
    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}