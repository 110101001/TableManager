#include<iostream>
#include "include/display.h"
#include "data.h"

using namespace std;

table **tables;

int main(int argc,char **argv){
	tables=new table*[10];
	for(int i=1;i<argc;i++){
		tables[i-1]=new table(argv[i]);
	}
	if(tables[0]==0){
		cout<<"Fatal Error: no input file!"<<endl;
		exit(0);
	}
	initDisplay();
	displayLoop();
	return 0;
}
