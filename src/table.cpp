#include "data.h"
#include <stdio.h>
#include <cstring>
#include <cmath>
#include "settings.h"
table::table(char *_fileName){
	FILE *file;
	int x=0,y=0;
	char fc;
	char *tempStr=new char[100];
	int strCount=0;
	bool inQuota=false;
	data **tempData;
	line *present;
	tableSize.col=0;
	tableSize.row=0;
	lineHead=0;
	file=fopen(_fileName,"r");

	while((fc=fgetc(file))!=EOF&&fc!='\n'){
		if(fc=='\"'){
			inQuota=!inQuota;
		}
		if(inQuota==false&&fc==','){
			tableSize.col++;
		}
	}
	tableSize.col++;
	items=new item[tableSize.col];

	rewind(file);
	while((fc=fgetc(file))!=EOF){
		if(fc=='\n'){
			tempStr[strCount++]=0;
			if(strCount!=0){
				items[y].str=new char[strCount];
				strcpy(items[y].str,tempStr);
				items[y].strLen=strCount;
				y++;
				strCount=0;
			}
			y=0;
			break;
		}
		else if(fc=='\"'){
			inQuota=!inQuota;
		}
		else if(inQuota==false&&fc==','){
			tempStr[strCount++]=0;
			if(strCount!=0){
				items[y].str=new char[strCount];
				strcpy(items[y].str,tempStr);
				items[y].strLen=strCount;
				y++;
				strCount=0;
			}
		}
		else{
			tempStr[strCount++]=fc;
		}
	}

	tempData=new data*[tableSize.col];

	while((fc=fgetc(file))!=EOF){
		if(fc=='\n'){
			tempStr[strCount++]=0;
			tempData[y]=readString(tempStr);
			y++;
			strCount=0;
			if(lineHead==0){
				lineHead=new line(tempData,y);
				present=lineHead;
				present->prev=0;
				present->next=0;
			}
			else{
				line *tempLine=new line(tempData,y);
				present->next=tempLine;
				tempLine->prev=present;
				present=present->next;
				present->next=0;
			}
			tempData=new data*[tableSize.col];
			y=0;
			x++;
		}
		else if(fc=='\"'){
			inQuota=!inQuota;
		}
		else if(inQuota==false&&fc==','){
			tempStr[strCount++]=0;
			tempData[y]=readString(tempStr);
			y++;
			strCount=0;
		}
		else{
			tempStr[strCount++]=fc;
		}	
	}
	tableSize.row=x;
	fileName=_fileName;
	index=0;
}

line *table::locateLine(int targetRow){
	if(targetRow>=tableSize.row){
		return 0;
	}
	int presentCount=0;
	line *present;
	if(index==0){
		present=lineHead;
		for(;presentCount<targetRow;presentCount++){
			present=present->next;
		}
	}
	else{
		while(presentCount>targetRow+2*INDEX_INTERVAL||presentCount<targetRow-2*INDEX_INTERVAL){
			int distance=targetRow-presentCount;
			double tempDepth=log10(distance);
			int depth=ceil(tempDepth);

		}
	}
	return present;
}

void table::updateIndex(){
	if(index==0){
		double depth=log10(tableSize.row);
		indexDepth=ceil(depth);
		index=new indexPointer*[indexDepth];
		index[0]->linePt=lineHead;
		index[0]->increase=0;
		for(int i=0;i<indexDepth;i++){
			index[i]->indexPt=index[i-1];
			index[i]->increase=0;
		}
	}
	indexPointer *present=index[0];
	while(present->next!=0){
		present=present->next;
		if(present->increase>2*INDEX_INTERVAL){
			indexPointer *temp=new indexPointer;

		}
	}
}
/*
int table::saveTable();//Save the table to it's original file, return 0 if success
int table::saveTable(char *fileName);//Save the table to certain file,

int table::insertItem(class item newItem);//insert a list of data, return 0 if success
int table::insertData(int position, class line);//insert a line of data, return 0 if success

int table::deleteItem(int position);//delete a list of data, return 0 if success
int table::deleteData(int position);//delete a line of data, return 0 if success

int table::sortByItem(int position,bool asending);//sort lines of data

int table::searchItem(char *name,int strLen);//search item by name, return position
class table *table::searchData(int itemPosition,class data,int lowerRange=0,int upperRange=0);//search by Data, return a table including all search results
class table *table::replaceData(int itemPosition,class data,class newData,int lowerRange=0,int upperRange=0);//replace data, return a table including all search results
*/
class line *table::operator[](int row){
	return locateLine(row);
}



