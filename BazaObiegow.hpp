#ifndef BAZAOBIEGOW_HPP
#define BAZAOBIEGOW_HPP
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "genalgpoc.hpp"
#include "pkpbaza.hpp"
#include "genalg.hpp"
#include <cmath>

class BazaObiegow
{
	vector <vector <Pociag*> > wynik;
	map <Pociag*, pair<int, int> > pozycjaPociagowWObiegach;
	set <string> typy;
	vector <pair <int, Pociag*> > pociagi_koncowe(StacjaKolejowa* stacja,  BazaRuchuKolejowego* bazab)
	{
		vector <pair <int, Pociag*> > wynik;
		RuchPociagowPrzezStacje* ruch = bazab->stacjaToRuch[stacja];
		for(int i=0; i<ruch->przejazdy.size(); i++)
		{
			if(ruch->przejazdy[i].second==NULL)
			{
				wynik.push_back(pair<int, Pociag*>(ruch->przejazdy[i].first->czasWyjazduZToru, ruch->przejazdy[i].first->pociag));
			}
		}
		return wynik;
	}

	vector <pair <int, Pociag*> > pociagi_poczatkowe(StacjaKolejowa* stacja,  BazaRuchuKolejowego* bazab)
	{
		vector <pair <int, Pociag*> > wynik;
		RuchPociagowPrzezStacje* ruch = bazab->stacjaToRuch[stacja];
		for(int i=0; i<ruch->przejazdy.size(); i++)
		{
			if(ruch->przejazdy[i].first==NULL)
			{
				wynik.push_back(pair<int, Pociag*>(ruch->przejazdy[i].second->czasWjazduNaTor, ruch->przejazdy[i].second->pociag));
			}
		}
		return wynik;
	}

	pair <Pociag*, Pociag*> odszukaj_najblizsze(map <Pociag*, int> kon, map <Pociag*, int> poc)
	{
		pair <Pociag*, Pociag*> wynik(NULL, NULL);
		int minwynik=2000;
		map <Pociag*, int>::iterator it1=kon.begin();
		int licznik=0;
		while(it1!=kon.end())
		{
			map <Pociag*, int>::iterator it2=poc.begin();
			while(it2!=poc.end())
			{
				int konakt=it1->second/60;
				int pocakt=it2->second/60;
				if((pocakt-konakt)>=3)
				{
					if((pocakt-konakt)<minwynik)
					{
						if(it1->first->getTypPociagu()==it2->first->getTypPociagu())
						{
							minwynik=pocakt-konakt;
							wynik=pair <Pociag*, Pociag*>(it1->first, it2->first);
						}
					}
				}
				it2++;
			}
			licznik++;
			it1++;
		}
		return wynik;
	}

	public:
	BazaObiegow(BazaInfryKolejowej* bazaa, BazaRuchuKolejowego* bazab)
	{
		map<Pociag*, Pociag*> nastepny;
		map<Pociag*, Pociag*> poprzedni;
		vector <StacjaKolejowa*> stacje = bazaa->wszystkieStacje();
		int s1=stacje.size();
		for(int i=0; i<s1; i++)
		{
			vector <pair <int, Pociag*> > pkon=pociagi_koncowe(stacje[i], bazab);
			vector <pair <int, Pociag*> > ppoc=pociagi_poczatkowe(stacje[i], bazab);
			map<Pociag*, int> pp;
			map<Pociag*, int> pk;
			int k11=pkon.size();
			for(int j=0; j<k11; j++)
			{
				nastepny[pkon[j].second]=NULL;
				pk[pkon[j].second]=pkon[j].first;
			}
			k11=ppoc.size();
			for(int j=0; j<k11; j++)
			{
				poprzedni[ppoc[j].second]=NULL;
				pp[ppoc[j].second]=ppoc[j].first;
			}
			pair <Pociag*, Pociag*> naj=odszukaj_najblizsze(pk, pp);
			int licznik=0;
			while(naj.first!=NULL)
			{
				licznik++;
				pk.erase(pk.find(naj.first));
				pp.erase(pp.find(naj.second));
				nastepny[naj.first]=naj.second;
				poprzedni[naj.second]=naj.first;
				naj=odszukaj_najblizsze(pk, pp);
			}
		}
		map <Pociag*, Pociag*>::iterator it1=poprzedni.begin();
		while(it1!=poprzedni.end())
		{
			if(it1->second==NULL)
			{
				vector <Pociag*> obieg;
				Pociag* aktu=it1->first;
				while(aktu!=NULL)
				{
					obieg.push_back(aktu);
					aktu=nastepny[aktu];
				}
				wynik.push_back(obieg);
			}
			it1++;
		}
		for(int i=0; i<wynik.size(); i++)
		{
			for(int j=0; j<wynik[i].size(); j++)
			{
				typy.insert(wynik[i][j]->getTypPociagu());
				cout<<wynik[i][j]->getTypPociagu()<<endl;
				pozycjaPociagowWObiegach[wynik[i][j]]=pair<int, int>(i, j);
			}
		}
	}
	int liczbaObiegow()
	{
		return wynik.size();
	}
	vector <Pociag*> getObieg(int i)
	{
		return wynik[i];
	}
	int ktoryObieg(Pociag* p)
	{
		return pozycjaPociagowWObiegach[p].first;
	}
	Pociag* nastepnyPociag(Pociag* poc)
	{
		pair<int, int> poz=pozycjaPociagowWObiegach[poc];
		if(wynik[poz.first].size()>poz.second+1)
			return wynik[poz.first][poz.second+1];
		return NULL;
	}
	Pociag* poprzedniPociag(Pociag* poc)
	{
		pair<int, int> poz=pozycjaPociagowWObiegach[poc];
		if(poz.second>0)
			return wynik[poz.first][poz.second-1];
		return NULL;
	}
	StacjaKolejowa* getStacjaPoczatkowa(int i)
	{
		return wynik[i][0]->getStacjaPoczatkowa();
	}
	StacjaKolejowa* getStacjaKoncowa(int i)
	{
		return wynik[i][wynik[i].size()-1]->getStacjaKoncowa();
	}
	vector <string> wszystkieTypy()
	{
		vector <string> wynik;
		set <string>::iterator it1=typy.begin();
		while(it1!=typy.end())
		{
			wynik.push_back(*it1);
			it1++;
		}
		return wynik;
	}
};

#endif
