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
	(*this)=rhs;
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
			if(isInt){
				return rhs.floatValue>intValue;
			}
			else{
				return rhs.intValue>floatValue;
			}
		}
	}
	else{
		if(str==0){
			return true;
		}
		else if(rhs.str==0){
			return false;
		}
		return strcmp(rhs.str,str)>0;
	}
	return false;//Shouldn't go here
}

bool data::operator==(const class data &rhs){
	if(rhs.isNum!=isNum){
		return false;
	}
	else if(isNum&&rhs.isNum){
		if(!(isInt||rhs.isInt)){
			return rhs.floatValue==floatValue;
		}
		else if(isInt&&rhs.isInt){
			return rhs.intValue==intValue;
		}
		else{
			if(isInt){
				return rhs.floatValue==intValue;
			}
			else{
				return rhs.intValue==floatValue;
			}
		}
	}
	else{
		if(str==0||rhs.str==0){
			if(rhs.str==0&&str==0){
				return true;
			}
			return false;
		}
		return strcmp(rhs.str,str)==0;
	}
	return false;//Shouldn't go here
}

bool data::operator>(const class data &rhs){
	if(rhs.isNum!=isNum){
		if(rhs.isNum){
			return false;
		}
		else{
			return true;
		}
	}
	else if(isNum&&rhs.isNum){
		if(!(isInt||rhs.isInt)){
			return rhs.floatValue<floatValue;
		}
		else if(isInt&&rhs.isInt){
			return rhs.intValue<intValue;
		}
		else{
			if(isInt){
				return rhs.floatValue<intValue;
			}
			else{
				return rhs.intValue<floatValue;
			}
		}
	}
	else{
		if(str==0){
			return false;
		}
		else if(rhs.str==0){
			return true;
		}
		return strcmp(rhs.str,str)<0;
	}
	return false;//Shouldn't go here
}

void data::operator=(const class data &rhs){
	if(rhs.isNum==false){
		isNum=false;
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
			if(strLen>MAX_DATA_LENGTH-1){
				strncpy(temp,str,MAX_DATA_LENGTH-4);
				temp[MAX_DATA_LENGTH-4]='.';
				temp[MAX_DATA_LENGTH-3]='.';
				temp[MAX_DATA_LENGTH-2]='.';
				temp[MAX_DATA_LENGTH-1]=0;
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

int* failCalc(char *pat){
	int *fail=new int[100];
	fail[0]=-1;
	for(int i=1;pat[i]!=0;i++){
		if(pat[i-1]==pat[fail[i-1]]){
			fail[i]=fail[i-1]+1;
		}
		else{
			fail[i]=0;
		}
	}	
	return fail;
}

int stringMatch(int *fail,char *pat,char *text){
	int patLen=strlen(pat);
	int textLen=strlen(text);
	for(int i=0;i<=textLen-patLen;i++){
		int j=0;
		while(j>=0&&i<textLen){
			if(text[i]==pat[j]){
				i++;
				j++;
				if(j==patLen){
					return i-patLen;
				}
			}
			else{
				j=fail[j];
			}
		}
	}
	return -1;
}

int data::replace(data &source,data *to){
	if(isNum==false&&str==0){
		return -1;
	}
	if(source.isNum!=true){
		return -1;
	}
	else{
		if(isInt==true){
			if(source.isInt!=true){
				return -1;
			}
			else{
				if(source.intValue==intValue){
					if(to!=0){
						(*this)=*to;
					}
					return 0;
				}
				else{
					return -1;
				}
			}
		}
		else{
			if(source.isInt==true){
				return -1;
			}
			else{
				if(source.floatValue==floatValue){
					if(to!=0){
						(*this)=*to;
					}
					return 0;
				}
				else{
					return -1;
				}
			}
		}
	}
}

int data::replace(data &source,data *to,int *fail){
	if(isNum==false&&str==0){
		return -1;
	}
	if(source.isNum==true||isNum==true){
		return -1;
	}
	else{
		int pos;
		if((pos=stringMatch(fail,source.str,str))!=-1){
			if(to!=0){
				while(pos!=-1){
					int increase=0;
					if(source.strLen==to->strLen){
						for(int i=pos;i<pos+source.strLen;i++){
							str[i]=to->str[i-pos];
						}
					}
					else{
						char *temp=new char[strLen+to->strLen-source.strLen];
						for(int i=0;i<pos;i++){
							temp[i]=str[i];
						}
						for(int i=pos;i<pos+to->strLen-1;i++){
							temp[i]=to->str[i-pos];
						}
						for(int i=pos+to->strLen-1;i<strLen+to->strLen-source.strLen;i++){
							temp[i]=str[i-to->strLen+source.strLen];
						}
						delete[] str;
						str=temp;
						strLen=strLen+to->strLen-source.strLen;
					}
					increase=stringMatch(fail,source.str,str+pos+to->strLen);
					if(increase==-1){
						break;
					}
					else{
						pos+=increase+to->strLen;
					}
				}
			}
			return 0;
		}
		else{
			return -1;
		}
	}
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
		char *newStr=new char[i+1];
		strcpy(newStr,s);
		temp=new class data(newStr,i+1);
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
		char *newStr=new char[i+1];
		strcpy(newStr,s);
		pt->editData(newStr,i+1);
	}
	return;
}

unsigned int bkdrHash(char *s){
	if(s==0){
		return 0;
	}
	unsigned int res=0;
	unsigned int factor=1;
	int seed=3;
	int pos=0;
	while(s[pos]!=0){
		res+=s[pos++]*factor;
		factor*=seed;
	}
	return res;
}

unsigned int data::hash(){
	if(isNum==false){
		return bkdrHash(str);
	}
	else{
		return 0;
	}
}
