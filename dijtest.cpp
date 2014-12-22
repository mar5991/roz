#include "BazaDijkstra.hpp"
using namespace std;
int main(int argc, char** argv)
{
	string sciez=argv[1];
	BazaInfryKolejowejFromFile ff(sciez);
	BazaInfryKolejowej* bazaa=new BazaInfryKolejowej(ff);
	int a, b;
	cin>>a>>b;
	vector <TorSzlakowy*> dip=dijkstra(bazaa->wszystkieStacje()[a], bazaa->wszystkieStacje()[b]);
	vector <StacjaKolejowa*> dip2=dijkstra2(bazaa->wszystkieStacje()[a], bazaa->wszystkieStacje()[b]);
	for(int i=0; i<dip.size(); i++)
	{
		cout<<"T "<<dip[i]->getId()<<endl;
	}
	for(int i=0; i<dip2.size()-1; i++)
	{
		cout<<"S "<<dip2[i]->getId()<<" "<<dip2[i]->getNazwa()<<endl;
		for(int j=0; j<dip[i]->getPrzystanki().size(); j++)
		{
			cout<<"P "<<dip[i]->getPrzystanki()[j].second->getId()<<" "<<dip[i]->getPrzystanki()[j].second->getNazwa()<<endl;
		}
	}
	cout<<"S "<<dip2[dip2.size()-1]->getId()<<" "<<dip2[dip2.size()-1]->getNazwa()<<endl;
}
