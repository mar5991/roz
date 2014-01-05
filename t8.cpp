#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "t6.hpp"
#include "cegla.hpp"
#include "genalgpoc.hpp"
using namespace std;
int main()
{
	string baza_sciezka;
	cin>>baza_sciezka;
	string abc;
	vector <string> abcx;
	baza_sieci bazaa(baza_sciezka);
	vector <int> wynik;
	int x;
	for(int i=0; i<8; i++)
	{
		cin>>x;
		wynik.push_back(x);
	}
	while(cin>>abc)
	{
		abcx.push_back(abc);
	}
	algopoc alg(8, 25, 5, 5, &bazaa, abcx);
	baza_pociagow bazab=alg.generuj(wynik);
	int as1=bazab.liczba_pociagow();
	vector <string> pociagi;
	for(int i=0; i<as1; i++)
	{
		pociagi.push_back(bazab.poc(i).kod);
		generujpociag(i, bazaa, bazab);
	}
	for(int i=0; i<bazaa.liczba_stacji(); i++)
	{
		generujstacje(bazaa, &bazab, i);
	}
	for(int j=0; j<bazaa.liczba_torow(); j++)
	{
		generujtorhtml(bazaa, &bazab, j);
		/*cout<<"TOR "<<bazaa.nazwa_stacji(bazaa.numer_stacji_pocz(j))<<" - "<<bazaa.nazwa_stacji(bazaa.numer_stacji_konc(j))<<endl;
		vector <pair <int, int> > czasy;
		vector <pociag_na_torze> alfa1=bazab.lista_toru_nor(j);
		vector <pociag_na_torze> alfa2=bazab.lista_toru_prz(j);
		int s1=alfa1.size();
		int s2=alfa2.size();
		for(int i=0; i<s1; i++)
		{
			czasy.push_back(pair<int, int>(alfa1[i].time_start, alfa1[i].time_stop));
		}
		for(int i=0; i<s2; i++)
		{
			czasy.push_back(pair<int, int>(alfa2[i].time_stop, alfa2[i].time_start));
		}
		sort(czasy.begin(), czasy.end());
		int s4=czasy.size();
		for(int i=0; i<s4; i++)
		{
			cout<<totimesec(czasy[i].first)<<" "<<totimesec(czasy[i].second)<<endl;
		}*/
	}
	vector <vector <string> > caboom=generujobiegi(bazaa, &bazab);
	generujzestawienie(caboom, bazaa, bazab);
	generuj_cegla2(bazaa, &bazab, pociagi, "data/cegla.html");
}
