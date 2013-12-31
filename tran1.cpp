#include "pkpbaza.hpp"
#include <iostream>
#include <cstring>
using namespace std;
int main(int argc, char** argv)
{
	char data[10000];
	int licznik=0;
	while(cin.getline(data, 10000))
	{
		cout<<argv[1]<<kodgen(licznik)<<" "<<0.8<<" "<<160<<" "<<20<<endl;
		string alfa=data;
		int s1=alfa.length();
		for(int i=0; i<s1; i++)
		{
			if(alfa[i]=='#')
				alfa[i]='\n';
		}
		cout<<alfa<<"\n";
		licznik++;
	}
}
