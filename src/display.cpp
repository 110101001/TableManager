#include "display.h"
#include "data.h"
#include <ncurses.h>
int maxRow;
int maxCol;
WINDOW *mainWindow;
WINDOW *commandWindow;

void initDisplay(){
	mainWindow=initscr();
	maxRow=LINES-1;
	maxCol=COLS-1;
	commandWindow=newwin(2,0,maxRow-2,0);	
	waddstr(mainWindow,temp.Str());
	cbreak();
	noecho();
	refresh();
}

void displayLoop(){
	char input;
	while(true){
		input=getch();
		switch(input){
			case 'q':
				endwin();
				return;
				break;
		}
		refresh();
	}
}
