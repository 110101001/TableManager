#include "display.h"
#include "data.h"
#include "settings.h"
#include <ncurses.h>
#include <iostream>

int tableWindowHeight;
int tableWindowWidth;
int x,y;
int curx,cury;
WINDOW *tableWindow;
WINDOW *commandWindow;
WINDOW *statusWindow;
table *displaying;

WINDOW *newBoxWin(int x,int y,int sx,int sy){
	WINDOW *temp;
	temp=newwin(x,y,sx,sy);
	box(temp,0,0);
	wrefresh(temp);
	return temp;
}

void initDisplay(){
	initscr();
	start_color();
	refresh();
	keypad(stdscr, TRUE);
	tableWindow=newBoxWin(LINES-3,COLS-STATUS_WIDTH,0,0);
	tableWindowWidth=(COLS-STATUS_WIDTH-2)/MAX_DATA_LENGTH;
	tableWindowHeight=LINES-4;
	commandWindow=newBoxWin(3,COLS,LINES-3,0);	
	statusWindow=newBoxWin(LINES-3,STATUS_WIDTH,0,COLS-STATUS_WIDTH);	
	cbreak();
	noecho();
}

line *displayTable(int x,int y){
	int posx=1;
	int posy=1;
	int edgey;
	werase(tableWindow);
	box(tableWindow,0,0);
	if(x>displaying->tableSize.row||y>displaying->tableSize.col){
		return 0;
	}
	if(y+tableWindowWidth>displaying->tableSize.col){
		edgey=displaying->tableSize.col;
	}
	else{
		edgey=y+tableWindowWidth;	
	}
	posy+=MAX_DATA_LENGTH;
	for(int i=y;i<edgey;i++){
		mvwaddstr(tableWindow,posx,posy+(i-y)*MAX_DATA_LENGTH,displaying->items[i]->str);
	}
	posx++;
	line *firstLine=(*displaying)[x];
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

void displayStatus(){
		char displayStr[STATUS_WIDTH];
		mvwaddstr(statusWindow,1,1,"Statistical Data");	
		sprintf(displayStr,"COLS:%d",displaying->tableSize.col);
		mvwaddstr(statusWindow,2,1,displayStr);	
		sprintf(displayStr,"LINES:%d",displaying->tableSize.row);
		mvwaddstr(statusWindow,3,1,displayStr);	
		sprintf(displayStr,"TotalDataNum:%d",displaying->tableSize.col*displaying->tableSize.row);
		mvwaddstr(statusWindow,4,1,displayStr);	
		wrefresh(statusWindow);
		return;	
}

void moveCur(int key){
	switch(key){
		case KEY_UP:
			if(curx+x==0){
				return;
			}
			else if(curx>0){
				curx--;
			}
			else{
				x--;
				displayTable(x,y);
			}
			break;
		case KEY_DOWN:
			if(curx+x>displaying->tableSize.row-1){
				return;
			}
			else if(curx<tableWindowHeight-2){
				curx++;
			}
			else{
				x++;
				displayTable(x,y);
			}
			break;
		case KEY_LEFT:
			if(cury+y==0){
				return;
			}
			else if(cury>0){
				cury--;
			}
			else{
				y--;
				displayTable(x,y);
			}
			break;
		case KEY_RIGHT:
			if(cury+y>displaying->tableSize.col-1){
				return;
			}
			else if(cury<tableWindowWidth-2){
				cury++;
			}
			else{
				y++;
				displayTable(x,y);
			}
			break;
	}
	wmove(tableWindow,1+curx,10*(1+cury));
	wrefresh(tableWindow);
}

void displayLoop(){
	int input;
	x=0,y=0;
	curx=0,cury=0;
	table test("test.csv");
	data **td=new data*[6];
	td[0]=new data("hahaha",7);
	td[1]=new data(192);
	td[2]=new data(60);
	td[3]=new data(3.7);
	td[4]=new data("Male",5);
	td[5]=new data(12);
	/*test.insertData(1000,new line(td,6));
	test.insertData(870,new line(td,6));
	test.insertData(400,new line(td,6));
	test.insertData(60,new line(td,6));
	test.insertData(123,new line(td,6));
	test.insertData(234,new line(td,6));
	test.insertData(924,new line(td,6));
	test.insertData(145,new line(td,6));
	test.insertData(650,new line(td,6));*/
	test.insertData(13,new line(td,6));
	test.updateIndex();
	displaying=&test;
	displayTable(x,y);
	displayStatus();
	
	while(true){
		input=getch();
		switch(input){
			case KEY_UP:
				moveCur(input);
				  break;
			case KEY_DOWN:
				moveCur(input);
				  break;
			case KEY_LEFT:
				moveCur(input);
				  break;
			case KEY_RIGHT:
				moveCur(input);
				  break;

			case ':':

				  break;
			case 'q':
				endwin();
				return;
				break;
		}
	}
}
