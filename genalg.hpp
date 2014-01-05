#ifndef GENALGHPP
#define GENALGHPP
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


struct pojosobnik
{
	vector <int> pz;
	int ocena;
};

bool sortx(pojosobnik alfa, pojosobnik beta)
{
	if(alfa.ocena>beta.ocena)
		return true;
	else
		return false;
}

class genalgo
{
	int losob;
	int lgen;
	int iwyw;
	int lmut;
	vector <pojosobnik> podzial;
	public:
	genalgo(int liczba_genow, int liczba_osobnikow, int ile_wywalic, int do_mutacji) : podzial(liczba_osobnikow)
	{
		losob=liczba_osobnikow;
		srand(time(NULL));
		lgen=liczba_genow;
		iwyw=ile_wywalic;
		for(int i=0; i<losob; i++)
		{
			pojosobnik nowy;
			vector <int> pz(lgen);
			for(int j=0; j<lgen; j++)
			{
				pz[j]=rand();
			}
			nowy.pz=pz;
			podzial[i]=nowy;
		}
	}
	vector <int> losuj(int &ocena)
	{
		vector <int> wynik;
		for(int i=0; i<losob; i++)
		{
			int al1=ocen(podzial[i].pz);
			podzial[i].ocena=al1;
		}
		sort(podzial.begin(), podzial.end(), sortx);
		ocena=podzial[0].ocena;
		for(int i=losob-1; i>=losob-iwyw; i--)
		{
			vector <int> pz(lgen);
			for(int j=0; j<lgen; j++)
			{
				pz[j]=rand();
			}
			podzial[i].pz=pz;
		}
		for(int i=0; i<losob-iwyw; i++)
		{
			int dolosowania=rand()%(losob-iwyw);
			for(int j=0; j<lgen; j++)
			{
				int rku=rand()%2;
				if(rku)
				{
					podzial[i].pz[j]=podzial[dolosowania].pz[j];
				}
			}
		}
		/*for(int i=0; i<lmut; i++)
		{
			//TODO
		}*/
		return podzial[0].pz;
	}
	virtual int ocen(vector <int> przyklad)
	{
		return 0;
	}
};
#endif
