#include<iostream>
#include<stdlib.h>

using namespace std;

int main(){
	cout<<"name,height,weight,grade,sex,age"<<endl;
	for(int i=0;i<1000;i++){
		cout<<"\"";
		for(int j=0;j<4;j++){
			int num=rand()%26;
			char output='a';
			output+=(char)num;
			cout<<output;
		}
		cout<<",";
		cout<<i+1;
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
