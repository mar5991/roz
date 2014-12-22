#ifndef BAZADIJKSTRA
#define BAZADIJKSTRA
#include "bazainfry.hpp"
#include <map>
#include <vector>
#include <algorithm>
map <StacjaKolejowa*, pair<int, TorSzlakowy*> > gdr(StacjaKolejowa* akt)
{
	map <StacjaKolejowa*, pair<int, TorSzlakowy*> > wynik;
	set <TorSzlakowy*> tory=akt->getToryWychodzace();
	auto it1=tory.begin();
	while(it1!=tory.end())
	{
		bool kierunek=0;
		if((*it1)->getStacjaPoczatkowa()==akt)
			kierunek=1;
		if(kierunek==0)
		{
			if(wynik.find((*it1)->getStacjaPoczatkowa())==wynik.end())
			{
				wynik[(*it1)->getStacjaPoczatkowa()]=pair<int, TorSzlakowy*>((*it1)->getDlugoscWMetrach(), *it1);
			}
		}
		else
		{
			wynik[(*it1)->getStacjaKoncowa()]=pair<int, TorSzlakowy*>((*it1)->getDlugoscWMetrach(), *it1);
		}
		it1++;
	}
	return wynik;
}
vector <TorSzlakowy*> dijkstra(StacjaKolejowa* start, StacjaKolejowa* stop)
{
	map <StacjaKolejowa*, int> dodane;
	map <StacjaKolejowa*, int> niedodane;
	map <StacjaKolejowa*, TorSzlakowy*> poprzednie;
	set <pair <int, StacjaKolejowa*> > kandydaci;
	kandydaci.insert(pair<int, StacjaKolejowa*>(0, start));
	niedodane[start]=0;
	while(!kandydaci.empty())
	{
		pair<int, StacjaKolejowa*> teraz=*kandydaci.begin();
		kandydaci.erase(kandydaci.begin());
		niedodane.erase(teraz.second);
		dodane[teraz.second]=teraz.first;
		map <StacjaKolejowa*, pair<int, TorSzlakowy*> > dorozpatrzenia=gdr(teraz.second);
		auto it1=dorozpatrzenia.begin();
		while(it1!=dorozpatrzenia.end())
		{
			StacjaKolejowa* next=it1->first;
			TorSzlakowy* tor=it1->second.second;
			int waga=it1->second.first;
			if(dodane.find(next)==dodane.end())
			{
				if(niedodane.find(next)==niedodane.end())
				{
					kandydaci.insert(pair<int, StacjaKolejowa*>(teraz.first+waga, next));
					niedodane[next]=(teraz.first+waga);
					poprzednie[next]=tor;
				}
				else
				{
					if(niedodane[next]>teraz.first+waga)
					{
						kandydaci.erase(kandydaci.find(pair<int, StacjaKolejowa*>(niedodane[next], next)));
						kandydaci.insert(pair<int, StacjaKolejowa*>(teraz.first+waga, next));
						niedodane[it1->first]=(teraz.first+waga);
						poprzednie[it1->first]=tor;
					}
				}
			}
			it1++;
		}
	}
	vector <TorSzlakowy*> wynik;
	StacjaKolejowa* akt=stop;
	while(akt!=start)
	{
		if(poprzednie[akt]==NULL)
		{
			reverse(wynik.begin(), wynik.end());
			return wynik;
		}
		wynik.push_back(poprzednie[akt]);
		if(akt==poprzednie[akt]->getStacjaPoczatkowa())
			akt=poprzednie[akt]->getStacjaKoncowa();
		else
			akt=poprzednie[akt]->getStacjaPoczatkowa();
	}
	reverse(wynik.begin(), wynik.end());
	return wynik;
}
vector <StacjaKolejowa*> dijkstra2(StacjaKolejowa* start, StacjaKolejowa* stop)
{
	map <StacjaKolejowa*, int> dodane;
	map <StacjaKolejowa*, int> niedodane;
	map <StacjaKolejowa*, TorSzlakowy*> poprzednie;
	set <pair <int, StacjaKolejowa*> > kandydaci;
	kandydaci.insert(pair<int, StacjaKolejowa*>(0, start));
	niedodane[start]=0;
	while(!kandydaci.empty())
	{
		pair<int, StacjaKolejowa*> teraz=*kandydaci.begin();
		kandydaci.erase(kandydaci.begin());
		niedodane.erase(teraz.second);
		dodane[teraz.second]=teraz.first;
		map <StacjaKolejowa*, pair<int, TorSzlakowy*> > dorozpatrzenia=gdr(teraz.second);
		auto it1=dorozpatrzenia.begin();
		while(it1!=dorozpatrzenia.end())
		{
			StacjaKolejowa* next=it1->first;
			TorSzlakowy* tor=it1->second.second;
			int waga=it1->second.first;
			if(dodane.find(next)==dodane.end())
			{
				if(niedodane.find(next)==niedodane.end())
				{
					kandydaci.insert(pair<int, StacjaKolejowa*>(teraz.first+waga, next));
					niedodane[next]=(teraz.first+waga);
					poprzednie[next]=tor;
				}
				else
				{
					if(niedodane[next]>teraz.first+waga)
					{
						kandydaci.erase(kandydaci.find(pair<int, StacjaKolejowa*>(niedodane[next], next)));
						kandydaci.insert(pair<int, StacjaKolejowa*>(teraz.first+waga, next));
						niedodane[it1->first]=(teraz.first+waga);
						poprzednie[it1->first]=tor;
					}
				}
			}
			it1++;
		}
	}
	vector <StacjaKolejowa*> wynik;
	StacjaKolejowa* akt=stop;
	wynik.push_back(akt);
	while(akt!=start)
	{
		if(poprzednie[akt]==NULL)
		{
			reverse(wynik.begin(), wynik.end());
			return wynik;
		}
		if(akt==poprzednie[akt]->getStacjaPoczatkowa())
			akt=poprzednie[akt]->getStacjaKoncowa();
		else
			akt=poprzednie[akt]->getStacjaPoczatkowa();
		wynik.push_back(akt);
	}
	reverse(wynik.begin(), wynik.end());
	return wynik;
}

#endif
