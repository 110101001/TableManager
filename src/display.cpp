#include "display.h"
#include "data.h"
#include "settings.h"
#include <ncurses.h>
#include <iostream>

int tableWindowHeight;
int tableWindowWidth;
WINDOW *tableWindow;
WINDOW *commandWindow;
WINDOW *statusWindow;

WINDOW *newBoxWin(int x,int y,int sx,int sy){
	WINDOW *temp;
	temp=newwin(x,y,sx,sy);
	box(temp,0,0);
	wrefresh(temp);
	return temp;
}

void initDisplay(){
	initscr();
	refresh();
	tableWindow=newBoxWin(LINES-3,COLS-STATUS_WIDTH,0,0);
	tableWindowWidth=(COLS-STATUS_WIDTH-2)/MAX_DATA_LENGTH;
	tableWindowHeight=LINES-4;
	commandWindow=newBoxWin(3,COLS,LINES-3,0);	
	statusWindow=newBoxWin(LINES-3,STATUS_WIDTH,0,COLS-STATUS_WIDTH);	
	cbreak();
	noecho();
}

line *displayTable(table &target,int x,int y){
	int posx=1;
	int posy=1;
	int edgey;
	if(x>target.tableSize.row||y>target.tableSize.col){
		return 0;
	}
	if(y+tableWindowWidth>target.tableSize.col){
		edgey=target.tableSize.col;
	}
	else{
		edgey=y+tableWindowWidth;	
	}
	posy+=MAX_DATA_LENGTH;
	for(int i=y;i<edgey;i++){
		mvwaddstr(tableWindow,posx,posy+(i-y)*MAX_DATA_LENGTH,target.items[i].str);
	}
	posx++;
	line *firstLine=target[x];
	line *present=firstLine;
	while(posx<tableWindowHeight&&present!=0){
		for(int i=y;i<edgey;i++){
			char *displayStr;
			displayStr=(*present)[i]->Str();
			mvwaddstr(tableWindow,posx,posy+(i-y)*MAX_DATA_LENGTH,displayStr);
			delete[] displayStr;
		}
		present=present->next;
		posx++;
	}
	for(int i=2;i<tableWindowHeight;i++){
		char displayStr[MAX_DATA_LENGTH];
		sprintf(displayStr,"%d",x+i-1);
		mvwaddstr(tableWindow,i,1,displayStr);
	}
	wrefresh(tableWindow);
	return firstLine;
}

void displayLoop(){
	char input;
	table test("test.csv");
	displayTable(test,1,0);
	while(true){
		input=getch();
		switch(input){
			case 'q':
				endwin();
				return;
				break;
		}
	}
}
