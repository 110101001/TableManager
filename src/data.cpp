#include "data.h"
#include "settings.h"
#include <stdlib.h>
#include <string.h>
#include <cstdio>

using namespace std;

data::data(char *_string,int _strLen):isNum(0),strLen(_strLen){
	str = (char *)malloc(strLen*sizeof(char));
	strcpy(str,_string);
	return;
}

data::data(const class data &rhs){
	if(rhs.isNum==false){
		if(rhs.str!=0){
			str=new char[rhs.strLen];
			strcpy(str,rhs.str);
			strLen=rhs.strLen;
		}
		else{
			str=0;
			strLen=0;
		}
	}
	else{
		isNum=true;
		if(rhs.isInt==false){
			isInt=false;
			floatValue=rhs.floatValue;
		}
		else{
			isInt=true;
			intValue=rhs.intValue;
		}
	}
		
}

data::~data(){
	if(isNum==false&&str!=0){
		delete[] str;
	}
}

bool data::operator<(const class data &rhs){
	if(rhs.isNum!=isNum){
		if(rhs.isNum){
			return true;
		}
		else{
			return false;
		}
	}
	else if(isNum&&rhs.isNum){
		if(!(isInt||rhs.isInt)){
			return rhs.floatValue>floatValue;
		}
		else if(isInt&&rhs.isInt){
			return rhs.intValue>intValue;
		}
		else{
			float a,b;
			if(isInt){
				return rhs.floatValue>intValue;
			}
			else{
				return rhs.intValue>floatValue;
			}
		}
	}
	else{
		return strcmp(rhs.str,str)>0;
	}
	return false;//Shouldn't go here
}

void data::editData(int val){
	if(isNum==false&&str!=0){
		delete[] str;
	}
	isNum=true;
	isInt=true;
	intValue=val;
}

void data::editData(double val){
	if(isNum==false&&str!=0){
		delete[] str;
	}
	isNum=true;
	isInt=false;
	floatValue=val;
}

void data::editData(char *_string,int strlen){
	if(isNum==false&&str!=0){
		delete[] str;
	}
	isNum=false;
	str=_string;
	strLen=strlen;
}

char *data::Str(){
	char *temp;
	temp=new char[MAX_DATA_LENGTH];
	if(isNum==false){
		if(str!=0){
			if(strLen>MAX_DATA_LENGTH){
				strncpy(temp,str,MAX_DATA_LENGTH-3);
				temp[MAX_DATA_LENGTH-3]='.';
				temp[MAX_DATA_LENGTH-2]='.';
				temp[MAX_DATA_LENGTH-1]='.';
				temp[MAX_DATA_LENGTH]=0;
			}
			else{
				strncpy(temp,str,MAX_DATA_LENGTH);
			}
		}
		else{
			temp[0]=0;
		}
	}
	else{
		if(isInt==false){
			snprintf(temp,MAX_DATA_LENGTH,"%f",floatValue);
		}
		else{
			snprintf(temp,MAX_DATA_LENGTH,"%d",intValue);
		}
	}
	return temp;
}

bool data::isEmpty(){
	if(isNum==false&&str==0){
		return true;
	}
	else{
		return false;
	}
}

class data *readString(char *s){
	int i;
	class data *temp;
	bool Num=true;
	bool point=false;
	for(i=0;s[i]!=0;i++){
		if((s[i]<'0'||s[i]>'9')&&s[i]!='.'){
			Num=false;
		}
		if(s[i]=='.'){
			point=true;
		}
	}
	if(Num==true){
		if(point==false){
			int intVal;
			sscanf(s,"%d",&intVal);
			temp=new class data(intVal);
		}
		else{
			double floatVal;
			sscanf(s,"%lf",&floatVal);
			temp=new class data(floatVal);
		}
	}
	else{
		char *newStr=new char[i];
		strcpy(newStr,s);
		temp=new class data(newStr,i);
	}
	return temp;
}

void readString(data *pt,char *s){
	int i;
	bool Num=true;
	bool point=false;
	for(i=0;s[i]!=0;i++){
		if((s[i]<'0'||s[i]>'9')&&s[i]!='.'){
			Num=false;
		}
		if(s[i]=='.'){
			point=true;
		}
	}
	if(Num==true){
		if(point==false){
			int intVal;
			sscanf(s,"%d",&intVal);
			pt->editData(intVal);
		}
		else{
			double floatVal;
			sscanf(s,"%lf",&floatVal);
			pt->editData(floatVal);
		}
	}
	else{
		char *newStr=new char[i];
		strcpy(newStr,s);
		pt->editData(newStr,i);
	}
	return;
}

