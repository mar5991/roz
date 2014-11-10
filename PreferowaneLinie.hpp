#ifndef PREFEROWANELINIE_HPP
#define PREFEROWANELINIE_HPP
#include "t6.hpp"
#include "bazainfry.hpp"
#include <set>
#include <map>
struct PreferowaneLinie
{
	BazaInfryKolejowej* baza;
	map <string, StacjaKolejowa*> stacje;
	map <pair<int, int>, int> idToruLaczacego;
	map<int, vector <int> > wychodzace; //id, tor
	set <int> przystanki;
	map <int, int> postojeNaStacjach;
	double przyspieszenie;
	int vmax;
	int czasPrzyst;
	int srodkowa;
	string kod1;
	string kod2;
	vector <int> stacjePosrednie(int start, int stop)
	{
		set <int> odwiedzone;
		vector <pair<int, int> > kolejka;
		int aktual=0;
		map <int, int> poprzednie;
		kolejka.push_back(pair<int, int>(start, 0));
		odwiedzone.insert(start);
		poprzednie[start]=-1;
		while(aktual!=kolejka.size())
		{
			int akt=kolejka[aktual].first;
			int poprz=kolejka[aktual].second;
			for(int i=0; i<wychodzace[akt].size(); i++)
			{
				if(odwiedzone.find(wychodzace[akt][i])==odwiedzone.end())
				{
					kolejka.push_back(pair<int,int>(wychodzace[akt][i],akt));
					poprzednie[wychodzace[akt][i]]=akt;
					odwiedzone.insert(wychodzace[akt][i]);
				}
			}
			aktual++;
		}
		vector <int> wynik;
		int akt=stop;
		while(akt!=-1)
		{
			wynik.push_back(akt);
			akt=poprzednie[akt];
		}
		reverse(wynik.begin(), wynik.end());
		return wynik;
	}
	public:
	vector <pair <int, int> > stacjePosrednie(string start, string stop)
	{
		vector <int> wynik1 = stacjePosrednie(stacje[start]->getId(), stacje[stop]->getId());
		vector <pair<int, int> > wynik;
		for(int i=0; i<wynik1.size(); i++)
		{
			wynik.push_back(pair<int, int>(wynik1[i], postojeNaStacjach[wynik1[i]]));
		}
		return wynik;
	}
	set <int> przystankiPosrednie(string start, string stop)
	{
		return przystanki;
	}
	vector <int> toryPosrednie(string start, string stop)
	{
		vector <int> wynik1 = stacjePosrednie(stacje[start]->getId(), stacje[stop]->getId());
		vector <int> wynik;
		for(int i=0; i<wynik1.size()-1; i++)
		{
			wynik.push_back(idToruLaczacego[pair<int, int>(wynik1[i], wynik1[i+1])]);
		}
		return wynik;
	}
	double getPrzyspieszenie()
	{
		return przyspieszenie;
	}
	int getvmax()
	{
		return vmax;
	}
	int getCzasPostojuPrzystanek()
	{
		return czasPrzyst;
	}
	int getStacjaSrodkowa()
	{
		return srodkowa;
	}
	string getKod1()
	{
		return kod1;
	}
	string getKod2()
	{
		return kod2;
	}
	PreferowaneLinie()
	{
	}
	PreferowaneLinie(BazaInfryKolejowej* bazaW, stringstream& plw)
	{
		baza=bazaW;
		vector <string> preferencesFiles;
		string kolejneNazwy;
		plw>>kod1;
		plw>>kod2;
		plw>>przyspieszenie;
		plw>>vmax;
		plw>>czasPrzyst;
		plw>>srodkowa;
		while(plw>>kolejneNazwy)
			preferencesFiles.push_back(kolejneNazwy);
		vector <StacjaKolejowa*> stacjeTeraz = baza->wszystkieStacje();
		for(int i=0; i<stacjeTeraz.size(); i++)
			stacje[stacjeTeraz[i]->getNazwa()]=stacjeTeraz[i];
		for(int i=0; i<preferencesFiles.size(); i++)
		{
			fstream plik(preferencesFiles[i].c_str());
			string typ;
			while(plik>>typ)
			{
				if(typ=="T")
				{
					int id;
					plik>>id;
					int id_start=baza->wszystkieTory()[id]->getStacjaPoczatkowa()->getId();
					int id_stop=baza->wszystkieTory()[id]->getStacjaKoncowa()->getId();
					idToruLaczacego[pair<int, int>(id_start, id_stop)]=id;
					idToruLaczacego[pair<int, int>(id_stop, id_start)]=id;
					wychodzace[id_start].push_back(id_stop);
					wychodzace[id_stop].push_back(id_start);

				}
				if(typ=="P")
				{
					int id;
					plik>>id;
					przystanki.insert(id);
				}
				if(typ=="S")
				{
					int id;
					plik>>id;
					int czas;
					plik>>czas;
					postojeNaStacjach[id]=czas;
				}
			}
			plik.close();
		}
	}
};
#endif
