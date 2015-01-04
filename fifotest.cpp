#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
//	mkfifo("/tmp/goodjazd", 0666);
	ifstream plik;
	plik.open("/tmp/goodjazd");
	while(1)
	{
		string typ;
		plik>>typ;
		cout<<"xD "<<typ<<endl;
		string ttt="/tmp/goodjazd"+typ+"b";
		ofstream plikwyj;
		plikwyj.open(ttt.c_str());
		string ttt2="/tmp/goodjazd"+typ+"a";
		ifstream plikwej;
		plikwej.open(ttt2.c_str());
		cout<<plikwyj.is_open()<<" "<<plikwej.is_open()<<endl;
		char znak=4;
		int godzina;
		plikwej>>godzina;
		cout<<godzina<<endl;
		//plikwyj<<"Content-type:text/plain"<<endl<<endl;
		plikwyj<<godzina<<endl;
		plikwyj<<znak<<endl;
		plikwyj.close();
		plikwej.close();
	}
}
