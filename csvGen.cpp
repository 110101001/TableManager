#include<iostream>
#include<stdlib.h>

using namespace std;

int main(){
	cout<<"id,name,height,weight,grade,sex,age"<<endl;
	for(int i=0;i<10000000;i++){
		cout<<i+1<<",";
		int len=4+rand()%6;
		int commaLen=2+rand()%2;
		cout<<"\"";
		for(int j=0;j<commaLen;j++){
			int num=rand()%26;
			char output='a';
			output+=(char)num;
			cout<<output;
		}
		cout<<",";
		for(int j=commaLen+1;j<len;j++){
			int num=rand()%26;
			char output='a';
			output+=(char)num;
			cout<<output;
		}
		cout<<"\",";

		int height=150+rand()%50;
		cout<<height<<",";
		int weight=45+rand()%45;
		cout<<weight<<",";
		float gpa=3+0.01*(rand()%200);
		cout<<gpa<<",";
		if(rand()%2==0){
			cout<<"Male,";
		}
		else{
			cout<<"Female,";
		}
		int age=15+rand()%10;
		cout<<age<<endl;
	}
	return 0;
}
