#include "data.h"
#include <iostream>

using namespace std;

line::line(){
	size=0;
	capicity=0;
	datas=0;
}

line::line(data **_datas,int _dataNum){
	size=_dataNum;
	capicity=_dataNum;
	datas=_datas;
}

line::line(int _capicity){
	capicity=_capicity;
	datas=new data*[capicity];
}

line::line(int _size,data repeat){
	size=_size;
	capicity=_size;
	datas=new data*[capicity];
	for(int i=0;i<size;i++){
		datas[i]=new data(repeat);
	}
}

line::~line(){
	delete[] datas;
}

data* line::operator[](int col){
	return datas[col];
}
