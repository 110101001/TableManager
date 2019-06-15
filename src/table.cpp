#include "data.h"
#include <stdio.h>
#include <cstring>
#include <cmath>
#include <fstream>
#include "settings.h"

using namespace std;

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
	items=new item*[tableSize.col];
	for(int i=0;i<tableSize.col;i++){
		items[i]=new item;
	}

	rewind(file);
	while((fc=fgetc(file))!=EOF){
		if(fc=='\n'){
			tempStr[strCount++]=0;
			if(strCount!=0){
				items[y]->str=new char[strCount];
				strcpy(items[y]->str,tempStr);
				items[y]->strLen=strCount;
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
				items[y]->str=new char[strCount];
				strcpy(items[y]->str,tempStr);
				items[y]->strLen=strCount;
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
	updateIndex();
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
		indexPointer *next;
		indexPointer *prev;
		next=index[indexDepth-1];
		prev=next;
		if(next->next!=0){
			next=next->next;
		}
		for(int i=indexDepth-1;i>=0;i--){
			while(next!=0&&presentCount+next->increase<=targetRow){
				presentCount+=next->increase;
				prev=next;
				next=next->next;
			}
			if(i==0){
				present=prev->linePt;
			}
			else{
				next=prev->indexPt;	
				prev=next;
				next=next->next;
			}
		}
		for(;presentCount<targetRow;presentCount++){
			present=present->next;
		}
	}
	return present;
}

void table::updateIndex(){
	if(index==0){
		double depth=log10(tableSize.row-1);
		depth/=log10(INDEX_INTERVAL);
		indexDepth=floor(depth);
		if(indexDepth==0){
			return;
		}
		index=new indexPointer*[indexDepth];
		for(int i=0;i<indexDepth;i++){
			index[i]=new indexPointer;
		}
		index[0]->linePt=lineHead;
		lineHead->parents=index[0];
		index[0]->increase=0;
		for(int i=1;i<indexDepth;i++){
			index[i]->indexPt=index[i-1];
			index[i-1]->parents=index[i];
			index[i]->increase=0;
		}
		index[indexDepth-1]->parents=0;
	}
	for(int i=0;i<indexDepth;i++){
		indexPointer *present=index[i];
		indexPointer *prevPresent;
		int presentCount=0;
		int interval=pow(INDEX_INTERVAL,i+1);
		while(present->next!=0&&presentCount+interval<tableSize.row){
			prevPresent=present;
			present=present->next;
			presentCount+=interval;

			int adjust = present->increase-interval;
			indexPointer *par=present;
			present->increase=interval;
			while(par->parents!=0){
				par=par->parents;
				par->increase-=adjust;
			}
			if(present->next!=0){
				present->next->increase+=adjust;
				par=present->next;
				while(par->parents!=0){
					par=par->parents;
					par->increase-=adjust;
				}
			}

			adjust/=(interval/INDEX_INTERVAL);
			if(adjust>0){
				if(i==0){
					for(;adjust!=0;adjust--){
						present->linePt->parents=present->next;
						present->linePt=present->linePt->prev;
					}
				}
				else{
					present->indexPt=prevPresent->indexPt;
					for(int j=0;j<INDEX_INTERVAL;j++){
						present->indexPt=present->indexPt->next;
					}
					for(int j=0;j<adjust;j++){
						indexPointer *temp=present->indexPt;
						temp=temp->next;
						temp->parents=present->next;
					}
				}
			}
			else if(adjust<0){
				if(i==0){
					for(;adjust!=0;adjust++){
						present->linePt=present->linePt->next;
						present->linePt->parents=present;
					}
				}
				else{
					for(;adjust!=0;adjust++){
						present->indexPt=present->indexPt->next;
						present->indexPt->parents=present;
					}
				}
			}
		}
		while(presentCount+interval<tableSize.row){
			present->next=new indexPointer;
			present->next->next=0;
			present->next->increase=interval;
			present->next->parents=0;
			if(i==0){
				present->next->linePt=present->linePt;
				for(int j=0;j<INDEX_INTERVAL;j++){
					present->next->linePt=present->next->linePt->next;
					present->next->linePt->parents=present->next;
				}
			}
			else{
				present->next->indexPt=present->indexPt;
				for(int j=0;j<INDEX_INTERVAL;j++){
					present->next->indexPt=present->next->indexPt->next;
					present->next->indexPt->parents=present->next;
				}
			}
			present=present->next;
			presentCount+=interval;
		}
	}
	return;
}

int table::searchItem(char *field){
	for(int i=0;i<tableSize.col;i++){
		if(items[i]->str!=0){
			if(strcmp(field,items[i]->str)==0){
				return i;
			}
		}
	}
	return -1;
}

int table::editItem(int position, char *field){
	int repeat;
	if((repeat=searchItem(field))!=-1&&repeat!=position){
		return -1;
	}
	items[position]->strLen=strlen(field);
	delete[] items[position]->str;
	items[position]->str=new char[items[position]->strLen];
	strcpy(items[position]->str,field);
	return 0;
}

int table::insertItem(int position,class item *newItem,data repeat){
	if(position>tableSize.col||position<0){
		return -1;
	}	
	tableSize.col++;
	item **temp=new item*[tableSize.col];
	for(int i=0;i<position;i++){
		temp[i]=items[i];
	}
	temp[position]=newItem;
	for(int i=position+1;i<tableSize.col;i++){
		temp[i]=items[i-1];
	}
	delete[] items;
	items=temp;
	line *present=lineHead;
	while(present!=0){
		if(present->insertData(position,new data(repeat))){
			return -1;
		}
		present=present->next;
	}
	return 0;
}


int table::insertData(int position, class line* newLine){
	if(position<0||position>tableSize.row){
		return -1;
	}
	else if(position==0){
		newLine->next=lineHead;
		newLine->prev=0;
		if(lineHead!=0){
			lineHead->prev=newLine;
		}
		lineHead=newLine;
		if(index!=0){
			lineHead->next->parents=index[0]->next;
			index[0]->linePt=newLine;
			indexPointer *present=index[0]->next;
			while(present!=0){
				present->increase++;
				present=present->parents;
			}
		}
		tableSize.row++;
		return 0;
	}
	else{
		int res=insertData(locateLine(position-1), newLine);
		return res;
	}
}

int table::insertData(class line* after, class line* newLine){
	line *before=after->next;
	after->next=newLine;
	newLine->prev=after;
	newLine->next=before;
	if(before!=0){
		before->prev=newLine;
		if(index!=0){
			indexPointer *present=before->parents;
			while(present!=0){
				present->increase++;
				present=present->parents;
			}
		}
	}
	tableSize.row++;
	return 0;
}

int table::deleteItem(int position){
	if(position>tableSize.col-1||position<0){
		return -1;
	}	
	tableSize.col--;
	item **temp=new item*[tableSize.col];
	for(int i=0;i<position;i++){
		temp[i]=items[i];
	}
	for(int i=position;i<tableSize.col;i++){
		temp[i]=items[i+1];
	}
	delete items[position];
	delete[] items;
	items=temp;
	line *present=lineHead;
	while(present!=0){
		if(present->deleteData(position)){
			return -1;
		}
		present=present->next;
	}
	return 0;
	
}

int table::deleteData(int position){
	if(position<0||position>tableSize.row-1){
		return -1;
	}
	else if(position==0){
		lineHead=lineHead->next;
		delete lineHead->prev;	
		if(index!=0){
			lineHead->parents=index[0];
			index[0]->linePt=lineHead;
			indexPointer *present=index[0]->next;
			while(present!=0){
				present->increase--;
				present=present->parents;
			}
		}
		tableSize.row--;
		return 0;
	}
	else{
		return deleteData(locateLine(position));
	}
}

int table::deleteData(class line* del){
	del->prev->next=del->next;
	if(del->next!=0){
		del->next->prev=del->prev;
		if(index!=0){
			indexPointer *present=del->next->parents;
			if(del->parents->linePt==del){
				del->parents->linePt=del->next;
				del->next->parents=del->parents;
			}
			while(present!=0){
				present->increase--;
				present=present->parents;
			}
		}
	}
	tableSize.row--;
	return 0;
}

template<class T>
void SWAP(T *&x,T *&y){
	T *tempPt;
	tempPt=x;
	x=y;
	y=tempPt;
}

void table::swapLine(line *a,line *b){
	bool changedA=false;
	if(a->next!=b){
		if(a->prev!=0){
			a->prev->next=b;
		}
		if(b->prev!=0){
			b->prev->next=a;
		}
		if(a->next!=0){
			a->next->prev=b;
		}
		if(b->next!=0){
			b->next->prev=a;
		}
		SWAP<line>(a->prev,b->prev);
		SWAP<line>(a->next,b->next);
	}
	else{
		if(a->prev!=0){
			a->prev->next=b;
		}
		if(b->next!=0){
			b->next->prev=a;
		}
		b->prev=a->prev;	
		a->prev=b;
		a->next=b->next;
		b->next=a;
	}
	SWAP<indexPointer>(a->parents,b->parents);
	if(a->parents!=0){
		if(a->parents->linePt==b){
			a->parents->linePt=a;
			changedA=true;
		}
	}
	if(b->parents!=0){
		if(b->parents->linePt==a&&(changedA==false||b->parents!=a->parents)){
			b->parents->linePt=b;
		}
	}
	if(lineHead==a){
		lineHead=b;
	}
	else if(lineHead==b){
		lineHead=a;
	}
}

void table::quickSort(line *begin,line *end,int position,bool assending){
	line *l=begin;
	line *r=end;
	line *lb=begin->prev;
	line *rb=end->next;
	if(begin==end){
		return;
	}
	while(l!=r){
		line *tempPt;
		if(!assending){
			while(l!=r&&!(*((*l)[position])<(*(*r)[position]))){
				r=r->prev;
			}
		}
		else{
			while(l!=r&&!(*((*r)[position])<(*(*l)[position]))){
				r=r->prev;
			}

		}
		if(l==r){
			break;
		}
		swapLine(l,r);
		SWAP<line>(l,r);
		if(begin==r){
			begin=l;
		}
		if(end==l){
			end=r;
		}
		l=l->next;
		if(!assending){
			while(l!=r&&!(*((*l)[position])<*((*r)[position]))){
				l=l->next;
			}
		}
		else{
			while(l!=r&&!(*((*r)[position])<*((*l)[position]))){
				l=l->next;
			}
		}
		if(l==r){
			break;
		}
		swapLine(l,r);
		SWAP<line>(l,r);
		if(begin==r){
			begin=l;
		}
		if(end==l){
			end=r;
		}
		r=r->prev;
	}
	if(l!=begin){
		quickSort(begin,l->prev,position,assending);
	}
	if(r!=end){
		quickSort(l->next,end,position,assending);
	}
}

int table::sortByItem(int position,bool assending){//QuickSort
	line *begin=lineHead;
	line *end=locateLine(tableSize.row-1);
	quickSort(begin,end,position,assending);
	return 0;
}

table *table::selectPart(int x,int y,int h,int w){
	if(x<0){
		h+=x;
		x=0;
	}
	if(y<0){
		w+=y;
		y=0;
	}
	if(h+x>tableSize.row){
		h=tableSize.row-x;
	}
	if(w+y>tableSize.col){
		w=tableSize.col-y;
	}
	if(h<=0||w<=0){
		return 0;
	}
	table *newTable=new table;
	newTable->tableSize={0,w};
	newTable->items=new item*[w];
	newTable->fileName=new char[strlen(fileName)];
	strcpy(newTable->fileName,fileName);
	for(int j=y;j<y+w;j++){
		item *temp=new item;
		temp->str=new char[items[j]->strLen];
		strcpy(temp->str,items[j]->str);
		temp->strLen=items[j]->strLen;
		newTable->items[j-y]=temp;
	}

	line *originalPresent=locateLine(x);
	data **temp=new data*[w];
	line *position;
	for(int i=0;i<h;i++){
		temp=new data*[w];
		for(int j=y;j<y+w;j++){
			temp[j-y]=new data(*((*originalPresent)[j]));
		}
		if(newTable->tableSize.row==0){
			newTable->insertData(i,new line(temp,1));
			position=newTable->lineHead;
		}
		else{
			newTable->insertData(position,new line(temp,1));
			position=position->next;
		}
		originalPresent=originalPresent->next;
	}
	newTable->updateIndex();
	return newTable;
}

bool hasComma(char *s){
	if(s==0){
		return false;
	}
	int i=0;
	while(s[i]!=0){
		if(s[i]==','){
			return true;
		}
		i++;
	}
	return false;
}

int table::saveTable(){
	ofstream file(fileName);
	for(int i=0;i<tableSize.col;i++){
		if(items[i]->str!=0){
			if(hasComma(items[i]->str)){
				file<<"\""<<items[i]->str<<"\"";
			}
			else{
				file<<items[i]->str;
			}
		}
		if(i==tableSize.col-1){
			file<<endl;
		}
		else{
			file<<",";
		}
	}
	line *present=lineHead;
	while(present!=0){
		for(int i=0;i<tableSize.col;i++){
			char *str=(*present)[i]->Str();
			if(hasComma(str)){
				file<<"\""<<str<<"\"";
			}
			else{
				file<<str;
			}
			if(i==tableSize.col-1){
				file<<endl;
			}
			else{
				file<<",";
			}
			delete[] str;
		}
		present=present->next;
	}
	file.close();
	return 0;
}
/*
int table::saveTable(char *fileName);//Save the table to certain file,
*/

class table *table::searchDataString(int itemPosition,class data source,int lowerRange,int upperRange){
	if(upperRange==0&&lowerRange==0){
		upperRange=tableSize.row-1;
	}
	if(lowerRange>=tableSize.row||upperRange<0||lowerRange>=upperRange||itemPosition>=tableSize.col){
		return 0;
	}
	if(lowerRange<0){
		lowerRange=0;
	}
	if(upperRange>=tableSize.row){
		upperRange=tableSize.row-1;
	}
	table *newTable=new table;
	newTable->tableSize={0,1};
	newTable->items=new item*[1];
	newTable->items[0]=new item;
	newTable->items[0]->str=new char[8];
	newTable->fileName=new char[strlen(fileName)];
	strcpy(newTable->fileName,fileName);
	strcpy(newTable->items[0]->str,"Matched");

	line *originalPresent=locateLine(lowerRange);
	line *position;
	if(source.isNum==false){
		int *failArray;
		failArray=failCalc(source.Str());
		if(itemPosition<0){
			for(int i=lowerRange;i<=upperRange;i++){
				for(int j=0;j<tableSize.col;j++){
					if((*originalPresent)[j]->replace(source,0,failArray)!=-1){
						data **temp=new data*[1];
						temp[0]=new data(*(*originalPresent)[j]);
						if(newTable->tableSize.row==0){
							newTable->insertData(newTable->tableSize.row,new line(temp,1));
							position=newTable->lineHead;
						}
						else{
							newTable->insertData(position,new line(temp,1));
							position=position->next;
						}
					}
				}
				originalPresent=originalPresent->next;
			}
		}
		else{
			for(int i=lowerRange;i<=upperRange;i++){
				if((*originalPresent)[itemPosition]->replace(source,0,failArray)!=-1){
					data **temp=new data*[1];
					temp[0]=new data(*(*originalPresent)[itemPosition]);
					if(newTable->tableSize.row==0){
						newTable->insertData(newTable->tableSize.row,new line(temp,1));
						position=newTable->lineHead;
					}
					else{
						newTable->insertData(position,new line(temp,1));
						position=position->next;
					}
				}
				originalPresent=originalPresent->next;
			}
		}
	}
	newTable->updateIndex();
	return newTable;
}

class table *table::searchDataNum(int itemPosition,class data source,int relation,int lowerRange,int upperRange){
	if(upperRange==0&&lowerRange==0){
		upperRange=tableSize.row-1;
	}
	if(lowerRange>=tableSize.row||upperRange<0||lowerRange>=upperRange||itemPosition>=tableSize.col){
		return 0;
	}
	if(lowerRange<0){
		lowerRange=0;
	}
	if(upperRange>=tableSize.row){
		upperRange=tableSize.row-1;
	}
	table *newTable=new table;
	newTable->tableSize={0,1};
	newTable->items=new item*[1];
	newTable->items[0]=new item;
	newTable->items[0]->str=new char[8];
	newTable->fileName=new char[strlen(fileName)];
	strcpy(newTable->fileName,fileName);
	strcpy(newTable->items[0]->str,"Matched");

	line *originalPresent=locateLine(lowerRange);
	line *position;
	if(source.isNum==true){
		int *failArray;
		failArray=failCalc(source.Str());
		if(itemPosition<0){
			for(int i=lowerRange;i<=upperRange;i++){
				for(int j=0;j<tableSize.col;j++){
					switch(relation){
						case -1:
							if((*(*originalPresent)[j])<source&&(*originalPresent)[j]->isNum==true){
								data **temp=new data*[1];
								temp[0]=new data(*(*originalPresent)[j]);
								if(newTable->tableSize.row==0){
									newTable->insertData(newTable->tableSize.row,new line(temp,1));
									position=newTable->lineHead;
								}
								else{
									newTable->insertData(position,new line(temp,1));
									position=position->next;
								}
							}
							break;
						case 1:
							if((*(*originalPresent)[j])>source&&(*originalPresent)[j]->isNum==true){
								data **temp=new data*[1];
								temp[0]=new data(*(*originalPresent)[j]);
								if(newTable->tableSize.row==0){
									newTable->insertData(newTable->tableSize.row,new line(temp,1));
									position=newTable->lineHead;
								}
								else{
									newTable->insertData(position,new line(temp,1));
									position=position->next;
								}
							}
							break;
						case 0:
							if((*(*originalPresent)[j])==source&&(*originalPresent)[j]->isNum==true){
								data **temp=new data*[1];
								temp[0]=new data(*(*originalPresent)[j]);
								if(newTable->tableSize.row==0){
									newTable->insertData(newTable->tableSize.row,new line(temp,1));
									position=newTable->lineHead;
								}
								else{
									newTable->insertData(position,new line(temp,1));
									position=position->next;
								}
							}
							break;
					}
				}
				originalPresent=originalPresent->next;
			}
		}
		else{
			for(int i=lowerRange;i<=upperRange;i++){
				switch(relation){
					case -1:
						if((*(*originalPresent)[itemPosition])<source&&(*originalPresent)[itemPosition]->isNum==true){
							data **temp=new data*[1];
							temp[0]=new data(*(*originalPresent)[itemPosition]);
							if(newTable->tableSize.row==0){
								newTable->insertData(newTable->tableSize.row,new line(temp,1));
								position=newTable->lineHead;
							}
							else{
								newTable->insertData(position,new line(temp,1));
								position=position->next;
							}
						}
						break;
					case 1:
						if((*(*originalPresent)[itemPosition])>source&&(*originalPresent)[itemPosition]->isNum==true){
							data **temp=new data*[1];
							temp[0]=new data(*(*originalPresent)[itemPosition]);
							if(newTable->tableSize.row==0){
								newTable->insertData(newTable->tableSize.row,new line(temp,1));
								position=newTable->lineHead;
							}
							else{
								newTable->insertData(position,new line(temp,1));
								position=position->next;
							}
						}
						break;
					case 0:
						if((*(*originalPresent)[itemPosition])==source&&(*originalPresent)[itemPosition]->isNum==true){
							data **temp=new data*[1];
							temp[0]=new data(*(*originalPresent)[itemPosition]);
							if(newTable->tableSize.row==0){
								newTable->insertData(newTable->tableSize.row,new line(temp,1));
								position=newTable->lineHead;
							}
							else{
								newTable->insertData(position,new line(temp,1));
								position=position->next;
							}
						}
						break;
				}
				originalPresent=originalPresent->next;
			}
		}
	}
	newTable->updateIndex();
	return newTable;
}

void table::replaceData(class data source,class data newData,int itemPosition,int lowerRange,int upperRange){
	if(upperRange==0&&lowerRange==0){
		upperRange=tableSize.row-1;
	}
	if(lowerRange>=tableSize.row||upperRange<0||lowerRange>=upperRange||itemPosition>=tableSize.col){
		return;
	}
	if(lowerRange<0){
		lowerRange=0;
	}
	if(upperRange>=tableSize.row){
		upperRange=tableSize.row-1;
	}
	line *present=locateLine(lowerRange);
	if(source.isNum==false){
		int *failArray;
		failArray=failCalc(source.Str());
		if(itemPosition<0){
			for(int i=lowerRange;i<=upperRange&&present!=0;i++){
				for(int j=0;j<tableSize.col;j++){
					(*present)[j]->replace(source,&newData,failArray);
				}
				present=present->next;
			}
		}
		else{
			for(int i=lowerRange;i<=upperRange&&present!=0;i++){
				(*present)[itemPosition]->replace(source,&newData,failArray);
				present=present->next;
			}
		}
		delete[] failArray;
	}
	else{
		if(itemPosition<0){
			for(int i=lowerRange;i<upperRange&&present!=0;i++){
				for(int j=0;j<tableSize.col;j++){
					(*present)[j]->replace(source,&newData);
				}
				present=present->next;
			}
		}
		else{
			for(int i=lowerRange;i<upperRange&&present!=0;i++){
				(*present)[itemPosition]->replace(source,&newData);
				present=present->next;
			}
		}
	}
}

unsigned int table::hash(){
	int count[8*sizeof(int)];
	unsigned int bit[8*sizeof(int)];
	unsigned int result=0;
	for(int i=0;i<8*sizeof(int);i++){
		count[i]=0;
		bit[i]=1<<i;
	}
	line *present=lineHead;
	while(present!=0){
		for(int j=0;j<tableSize.col;j++){
			int temp=(*present)[j]->hash();
			if(temp!=0){
				for(int i=0;i<8*sizeof(int);i++){
					if((temp&bit[i])!=0){
						count[i]++;
					}
					else{
						count[i]--;
					}
				}
			}
		}
		present=present->next;
	}
	for(int i=0;i<8*sizeof(int);i++){
		if(count[i]>0){
			result+=bit[i];
		}
	}
	return result;
}

class line *table::operator[](int row){
	return locateLine(row);
}



