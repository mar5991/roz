#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
using namespace std;
struct tor
{
	int dlugosc;
	int from;
	int to;
	int vmax;
	int nast1;
	int nast2;
	string desc;
	vector <pair<int, int> > przystanki;
	void wczytaj(stringstream& alfa)
	{
		alfa>>dlugosc;
		alfa>>from;
		alfa>>to;
		alfa>>vmax;
		alfa>>nast1;
		alfa>>nast2;
		int odl1;
		cerr<<"TOR "<<endl;
		while(alfa>>odl1)
		{
			if(odl1==-1)
			{
				string cos1;
				while(alfa>>cos1)
				{
					desc+=cos1+" ";
				}
			}
			else
			{
				int id;
				alfa>>id;
				przystanki.push_back(pair<int, int>(odl1, id));
			}
		}
	}
	void wypisz()
	{
		cout<<"T "<<dlugosc<<" ";
		cout<<from<<" ";
		cout<<to<<" ";
		cout<<vmax<<" ";
		cout<<nast1<<" ";
		cout<<nast2<<" ";
		for(int i=0; i<przystanki.size(); i++)
		{
			cout<<przystanki[i].first<<" "<<przystanki[i].second<<" ";
		}
		if(desc!="")
		{
			cout<<"-1 "<<desc;
		}
		cout<<endl;
	}
};
vector <string> bazaStacji0;
map <string, int> bazaStacji1;
vector <string> bazaPrzystankow0;
map <string, int> bazaPrzystankow1;
vector <tor> tory;
int getIdPrzystanku(string nazwa)
{
	if(bazaPrzystankow1.find(nazwa)==bazaPrzystankow1.end())
	{
		bazaPrzystankow1[nazwa]=bazaPrzystankow0.size();
		bazaPrzystankow0.push_back(nazwa);
	}
	return bazaPrzystankow1[nazwa];
}
int getIdStacji(string nazwa)
{
	if(bazaStacji1.find(nazwa)==bazaStacji1.end())
	{
		bazaStacji1[nazwa]=bazaStacji0.size();
		bazaStacji0.push_back(nazwa);
	}
	return bazaStacji1[nazwa];

}
void dodajTorNowy(vector <pair<int, string> > nazwy, int vmax, int nast1, int nast2)
{
	tor nowy;
	nowy.vmax=vmax;
	nowy.nast1=nast1;
	nowy.nast2=nast2;
	nowy.dlugosc=abs(nazwy[nazwy.size()-1].first-nazwy[0].first);
	nowy.from=getIdStacji(nazwy[0].second);
	nowy.to=getIdStacji(nazwy[nazwy.size()-1].second);
	for(int i=1; i<(nazwy.size()-1); i++)
	{
		nowy.przystanki.push_back(pair<int, int>(abs(nazwy[i].first-nazwy[0].first), getIdPrzystanku(nazwy[i].second)));
	}
	tory.push_back(nowy);
}
void wczytajStacje(stringstream& alfa)
{
	string nazwa;
	alfa>>nazwa;
	bazaStacji1[nazwa]=bazaStacji0.size();
	bazaStacji0.push_back(nazwa);
}
void wczytajPrzystanek(stringstream& alfa)
{
	string nazwa;
	alfa>>nazwa;
	bazaPrzystankow1[nazwa]=bazaPrzystankow0.size();
	bazaPrzystankow0.push_back(nazwa);
}
void wczytaj(string sciez)
{
	char data[10000];
	fstream plik(sciez.c_str());
	while(!plik.eof())
	{
		plik.getline(data, 10000);
		stringstream foo;
		foo<<data;
		string typ;
		foo>>typ;
		if(typ=="S")
			wczytajStacje(foo);
		if(typ=="P")
			wczytajPrzystanek(foo);
		if(typ=="T")
		{
			tor nowy;
			nowy.wczytaj(foo);
			tory.push_back(nowy);
		}
	}
}
void wypisz()
{
	for(int i=0; i<bazaStacji0.size(); i++)
		cout<<"S "<<bazaStacji0[i]<<endl;
	for(int i=0; i<bazaPrzystankow0.size(); i++)
		cout<<"P "<<bazaPrzystankow0[i]<<endl;
	for(int i=0; i<tory.size(); i++)
	{
		tory[i].wypisz();
	}
}
string getNazwa(stringstream& reszta)
{
	string wynik;
	vector <string> lnn;
	string tmp;
	while(reszta>>tmp)
	{
		lnn.push_back(tmp);
	}
	for(int i=0; i<lnn.size(); i++)
	{
		wynik+=lnn[i];
		if(i<lnn.size()-1)
			wynik+="_";
	}
	string wynik2;
	for(int i=0; i<wynik.length(); i++)
	{
		if(i<wynik.length()-1 && wynik[i]==-62 && wynik[i+1]==-96)
		{
			i++;
		}
		else
		{
			wynik2+=wynik[i];
		}
	}
	return wynik2;
}
void wczytajNowe(string sciez)
{
	vector <pair<int, string> > nazwy;
	int liczba_torow;
	int vmax;
	int nast1;
	int nast2;
	string poprzednia_stacja;
	fstream plik(sciez.c_str());
	while(!plik.eof())
	{
		cerr<<"a jednak"<<endl;
		char data[10000];
		plik.getline(data,10000);
		stringstream dtx;
		dtx<<data;
		string type;
		dtx>>type;
		if(type=="S")
		{
			string nazwa;
			int nowa_liczba_torow;
			int nowe_v_max;
			int nowe_n1;
			int nowe_n2;
			int odl;
			dtx>>nowa_liczba_torow;
			dtx>>nowe_v_max;
			dtx>>nowe_n1;
			dtx>>nowe_n2;
			dtx>>odl;
			nazwa=getNazwa(dtx);
			cerr<<nazwa<<endl;
			nazwy.push_back(pair<int, string>(odl, nazwa));
			if(nazwy.size()>1)
			{
				dodajTorNowy(nazwy, vmax, nast1, nast2);
				if(liczba_torow==2)
				{
					reverse(nazwy.begin(), nazwy.end());
					dodajTorNowy(nazwy, vmax, nast1, nast2);
				}
			}
			nazwy.clear();
			liczba_torow=nowa_liczba_torow;
			vmax=nowe_v_max;
			nast1=nowe_n1;
			nast2=nowe_n2;
			nazwy.push_back(pair<int, string>(odl, nazwa));
		}
		if(type=="P")
		{
			string nazwa;
			int odl;
			dtx>>odl;
			nazwa=getNazwa(dtx);
			nazwy.push_back(pair<int, string>(odl, nazwa));
		}
	}
}
int main(int argc, char** argv)
{
	string sciez=argv[1];
	wczytaj(sciez);
	for(int i=2; i<argc; i++)
	{
		sciez=argv[i];
		wczytajNowe(sciez);
	}
	wypisz();
}
