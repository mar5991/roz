#include "bazainfry.hpp"
using namespace std;
int main(int argc, char** argv)
{
	string sciez=argv[1];
	BazaInfryKolejowejFromFile ff(sciez);
	BazaInfryKolejowej* bazaa=new BazaInfryKolejowej(ff);
	string cos;
	while (cin>>cos)
	{
		int b;
		cin>>b;
		TorSzlakowy* teraz=bazaa->wszystkieTory()[a];
	}
	for(int i=0; i<dip.size(); i++)
	{
		cout<<"T "<<dip[i]->getId()<<endl;
	}
}
