#ifndef LICZBAPOCIAGOWSTACJA_HPP
#define LICZBAPOCIAGOWSTACJA_HPP
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "genalgpoc.hpp"
#include "pkpbaza.hpp"
#include "genalg.hpp"
#include "BazaObiegow.hpp"
class LiczbaPociagowStacja
{
	map <string, vector <int> > postojeObiegowe;
	map <string, vector <int> > postojeKrancowe;
	map <string, vector <int> > postojeStacyjne;
	set <string> typy;
	int maxPostojeKrancowe;
	int maxPostojeStacyjne;
	void zerujVector (vector <int>& foo)
	{
		for(int i=0; i<foo.size(); i++)
		{
			foo[i]=0;
		}
	}
	void initZmienne()
	{
		maxPostojeKrancowe=0;
		maxPostojeStacyjne=0;
		for(int i=0; i<DLUGOSC_DNIA; i++)
		{
			maxPostojeKrancowe=max(maxPostojeKrancowe, zajetoscTorowOdstawczych(i));
			maxPostojeStacyjne=max(maxPostojeStacyjne, zajetoscWszystkichTorow(i));
		}
	}
	void initTypy(RuchPociagowPrzezStacje* ruch)
	{
		int s1 = ruch->przejazdy.size();
		for(int i=0; i<s1; i++)
		{
			Pociag* poc;
			if(ruch->przejazdy[i].first==NULL)
				poc=ruch->przejazdy[i].second->pociag;
			else
				poc=ruch->przejazdy[i].first->pociag;
			typy.insert(poc->getTypPociagu());
		}
		set <string>::iterator it1 = typy.begin();
		while(it1!=typy.end())
		{
			postojeObiegowe[*it1]=vector <int>(DLUGOSC_DNIA);
			postojeKrancowe[*it1]=vector <int>(DLUGOSC_DNIA);
			postojeStacyjne[*it1]=vector <int>(DLUGOSC_DNIA);
			zerujVector(postojeObiegowe[*it1]);
			zerujVector(postojeKrancowe[*it1]);
			zerujVector(postojeStacyjne[*it1]);
			it1++;
		}
	}
	void initPociagObiegStart(Pociag* p)
	{
		string typ = p->getTypPociagu();
		for(int i=0; i<p->getCzasStart(); i++)
		{
			postojeObiegowe[typ][i]++;
		}
	}
	void initPociagObiegStop(Pociag* p)
	{
		string typ = p->getTypPociagu();
		for(int i=p->getCzasStop(); i<DLUGOSC_DNIA; i++)
		{
			postojeObiegowe[typ][i]++;
		}
	}
	void initPociagKraniec(Pociag* p, Pociag* q)
	{
		string typ = p->getTypPociagu();
		for(int i=p->getCzasStop(); i<=min(q->getCzasStart(), DLUGOSC_DNIA-1); i++)
		{
			postojeKrancowe[typ][i]++;
		}
	}
	void initPociagStacyjny(Pociag* p, int wjazd, int wyjazd)
	{
		string typ = p->getTypPociagu();
		for(int i=wjazd; i<=min(wyjazd, DLUGOSC_DNIA-1); i++)
		{
			postojeStacyjne[typ][i]++;
		}
	}
	void initPostojeObiegowe(RuchPociagowPrzezStacje* ruch, BazaObiegow& bazac)
	{
		int s1 = ruch->przejazdy.size();
		for(int i=0; i<s1; i++)
		{
			if(ruch->przejazdy[i].first==NULL)
			{
				Pociag* poc=ruch->przejazdy[i].second->pociag;
				if(bazac.poprzedniPociag(poc)==NULL)
				{
					initPociagObiegStart(poc);
				}
			}
			if(ruch->przejazdy[i].second==NULL)
			{
				Pociag* poc=ruch->przejazdy[i].first->pociag;
				if(bazac.nastepnyPociag(poc)==NULL)
				{
					initPociagObiegStop(poc);
				}
				cout<<poc<<endl;
			}
		}
	}
	void initPostojeKrancowe(RuchPociagowPrzezStacje* ruch, BazaObiegow& bazac)
	{
		int s1=ruch->przejazdy.size();
		for(int i=0; i<s1; i++)
		{
			if(ruch->przejazdy[i].second==NULL)
			{
				Pociag* poc=ruch->przejazdy[i].first->pociag;
				if(bazac.nastepnyPociag(poc)!=NULL)
				{
					Pociag* poc2=bazac.nastepnyPociag(poc);
					initPociagKraniec(poc, poc2);
				}
				cout<<poc<<endl;
			}
		}
	}
	void initPostojeStacyjne(RuchPociagowPrzezStacje* ruch, BazaObiegow& bazac)
	{
		int s1=ruch->przejazdy.size();
		for(int i=0; i<s1; i++)
		{
			if(ruch->przejazdy[i].second!=NULL && ruch->przejazdy[i].first!=NULL)
			{
				Pociag* poc=ruch->przejazdy[i].first->pociag;
				initPociagStacyjny(poc, ruch->przejazdy[i].first->czasWyjazduZToru, ruch->przejazdy[i].second->czasWjazduNaTor);
			}
		}
	}
	public:
	static const int DLUGOSC_DNIA = 60*60*30;
	LiczbaPociagowStacja(BazaObiegow& bazac, StacjaKolejowa* stanica, BazaInfryKolejowej* bazaa, BazaRuchuKolejowego* bazab)
	{
		RuchPociagowPrzezStacje* ruch = bazab->stacjaToRuch[stanica];
		initTypy(ruch);
		initPostojeObiegowe(ruch, bazac);
		initPostojeKrancowe(ruch, bazac);
		initPostojeStacyjne(ruch, bazac);
		initZmienne();
	}
	int obiegiPoczatkowe(string typ)
	{
		if(postojeObiegowe.find(typ)==postojeObiegowe.end())
			return 0;
		return postojeObiegowe[typ][0];
	}
	int zajetoscTorowOdstawczych(string typ, int time)
	{
		if(postojeObiegowe.find(typ)==postojeObiegowe.end())
			return 0;
		return postojeObiegowe[typ][time]+postojeKrancowe[typ][time];
	}
	int zajetoscWszystkichTorow(string typ, int time)
	{
		if(postojeObiegowe.find(typ)==postojeObiegowe.end())
			return 0;
		return postojeObiegowe[typ][time]+postojeKrancowe[typ][time]+postojeStacyjne[typ][time];
	}
	int maksymalnaZajetoscOdstawczych()
	{
		return maxPostojeKrancowe;
	}
	int maksymalnaZajetoscWszystkich()
	{
		return maxPostojeStacyjne;
	}
	int zajetoscTorowOdstawczych(int time)
	{
		int wynik=0;
		for(auto& it1 : typy)
		{
			wynik+=zajetoscTorowOdstawczych(it1, time);
		}
		return wynik;
	}
	int zajetoscWszystkichTorow(int time)
	{
		int wynik=0;
		for(auto& it1 : typy)
		{
			wynik+=zajetoscWszystkichTorow(it1, time);
		}
		return wynik;
	}
	int obiegiKoncowe(string typ)
	{
		if(postojeObiegowe.find(typ)==postojeObiegowe.end())
			return 0;
		return postojeObiegowe[typ][DLUGOSC_DNIA-1];
	}
	int obiegiPoczatkowe()
	{
		int wynik=0;
		map<string, vector <int> >::iterator it1=postojeObiegowe.begin();
		while(it1!=postojeObiegowe.end())
		{
			wynik+=obiegiPoczatkowe(it1->first);
			it1++;
		}
		return wynik;
	}
	int obiegiKoncowe()
	{
		int wynik=0;
		map<string, vector <int> >::iterator it1=postojeObiegowe.begin();
		while(it1!=postojeObiegowe.end())
		{
			wynik+=obiegiKoncowe(it1->first);
			it1++;
		}
		return wynik;
	}
};

#endif
