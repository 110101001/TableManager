#include<iostream>
#include<stdlib.h>
#include<string>

using namespace std;

int main(){
	int op;
	int x=100000;
	int y=7;
	for(int i=0;i<100;i++){
		op=rand()%7;
		switch(op){
			case 0://jump
				cout<<":j"<<endl;
				int line=rand()%x+1;
				cout<<line<<endl;
				break;
			case 1://replace
				char match[2];
				char replace[3];
				match[0]=rand()%26+'a';	
				match[1]=rand()%26+'a';	
				replace[0]=rand()%26+'a';	
				replace[1]=rand()%26+'a';	
				replace[2]=rand()%26+'a';	
				cout<<:
		}
	}
	return 0;
}
