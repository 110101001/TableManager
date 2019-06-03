#include "data.h"
#include <iostream>

using namespace std;

line::line(){
	size=0;
	capicity=0;
	datas=0;
	parents=0;
}

line::line(data **_datas,int _dataNum){
	size=_dataNum;
	capicity=_dataNum;
	datas=_datas;
	parents=0;
}

line::line(int _capicity){
	size=0;
	capicity=_capicity;
	datas=new data*[capicity];
	parents=0;
}

line::line(int _size,data repeat){
	size=_size;
	capicity=_size;
	datas=new data*[capicity];
	for(int i=0;i<size;i++){
		datas[i]=new data(repeat);
	}
	parents=0;
}

line::~line(){
	delete[] datas;
}
int line::insertData(int pos,class data* dataPt){
	if(pos>size||pos<0){
		return -1;
	}
	if(capicity>=size+1){
		for(int i=size;i>pos;i--){
			datas[i]=datas[i-1];
		}
		datas[pos]=dataPt;
		size++;
	}
	else{
		capicity++;
		size++;
		data **temp=new data*[capicity];
		for(int i=0;i<pos;i++){
			temp[i]=datas[i];
		}
		temp[pos]=dataPt;
		for(int i=pos+1;i<capicity;i++){
			temp[i]=datas[i-1];
		}
		delete[] datas;
		datas=temp;
	}
	return 0;
}

int line::deleteData(int pos){
	if(pos>size){
		return -1;
	}
	size--;
	for(int i=pos;i<size-1;i++){
		datas[i]=datas[i+1];
	}	
	return 0;
}

data* line::operator[](int col){
	return datas[col];
}
