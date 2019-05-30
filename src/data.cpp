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

data::~data(){
	if(isNum==false){
		free(str);
	}
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
