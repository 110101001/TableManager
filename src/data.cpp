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
		str=new char[rhs.strLen];
		strcpy(str,rhs.str);
		strLen=rhs.strLen;
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
	temp=(char *)malloc(sizeof(char)*MAX_DATA_LENGTH);
	if(isNum==false){
		strncpy(temp,str,MAX_DATA_LENGTH);
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
			break;
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
