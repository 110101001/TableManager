#include "display.h"
#include "data.h"
#include "settings.h"
#include <ncurses.h>
#include <cstring>
#include <iostream>

using namespace std;

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
		if(displaying->items[i]->str!=0){
			mvwaddstr(tableWindow,posx,posy+(i-y)*MAX_DATA_LENGTH,displaying->items[i]->str);
		}
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
	wmove(tableWindow,1+curx,1+10*(1+cury));
	wrefresh(tableWindow);
}

void Err(){
			mvwaddstr(commandWindow,1,1,"Out of range! Are you trying to trigger a segementation fault?");	
			wrefresh(commandWindow);
}

int commandLine(){
	char input[100];
	char *emptyLine=new char[COLS-1];
	for(int i=0;i<COLS;i++){
		emptyLine[i]=' ';
	}
	emptyLine[COLS-2]=0;
	mvwaddstr(commandWindow,1,1,emptyLine);	
	mvwaddstr(commandWindow,1,1,":");	
	wmove(commandWindow,1,2);
	wrefresh(commandWindow);
	nocbreak();
	echo();
	wscanw(commandWindow,"%s",&input);
	if(0==strcmp(input,"q")){
		return -1;
	}
	else if(0==strcmp(input,"j")){
		int position;
		mvwaddstr(commandWindow,1,1,emptyLine);	
		mvwaddstr(commandWindow,1,1,"Jump to:");
		wscanw(commandWindow,"%d",&position);
		if(position<=displaying->tableSize.row&&position>0){
			curx=1;
			cury=0;
			x=position-1;
			wmove(tableWindow,1+curx,1+10*(1+cury));
			displayTable(x,y);
		}
		else{
			Err();
		}
	}
	else if(0==strcmp(input,"sort")){
		int position;
		int ascending;
		mvwaddstr(commandWindow,1,1,emptyLine);	
		mvwaddstr(commandWindow,1,1,"Select column and ascending:");
		wscanw(commandWindow,"%d %d",&position,&ascending);
		if(position<=displaying->tableSize.col&&position>0){
			mvwaddstr(commandWindow,1,1,emptyLine);	
			displaying->sortByItem(position-1,ascending!=0);
			displayTable(x,y);
		}
		else{
			Err();
		}
	}
	else if(0==strcmp(input,"update")){
		displaying->updateIndex();
	}
	cbreak();
	noecho();
	return 0;
}

void displayLoop(){
	int input;
	data emptyData;
	x=0,y=0;
	curx=0,cury=0;
	table test("test.csv");
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
			case 'd':
				input=getch();
				switch(input){
					case 'l':
						if(curx+x!=0){
							displaying->deleteData(curx+x-1);
						}
						break;
					case 'c':
						displaying->deleteItem(cury+y);
						break;
				}
				displayTable(x,y);
				break;
			case 'n':
				input=getch();
				switch(input){
					case 'l':
						displaying->insertData(curx+x,new line(displaying->tableSize.col,emptyData));
						break;
					case 'c':
						displaying->insertItem(cury+y,new item,emptyData);
						break;
				}
				displayTable(x,y);
				break;
			case 'e':
				char s[100];
				nocbreak();
				echo();
				mvwaddstr(tableWindow,1+curx,1+(1+cury)*MAX_DATA_LENGTH,"          ");
				mvwscanw(tableWindow,1+curx,1+(1+cury)*MAX_DATA_LENGTH,"%s",&s);
				readString((*(*displaying)[curx+x-1])[cury+y],(char *)s);
				cbreak();
				noecho();
				displayTable(x,y);
				break;
			case ':':
				if(commandLine()==-1){
					endwin();
				return;
				}
				break;
		}
	}
}
