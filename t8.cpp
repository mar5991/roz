#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "t6.hpp"
/*#include "cegla.hpp"*/
#include "genalgpoc.hpp"
using namespace std;
int main()
{
	string baza_sciezka;
	cin>>baza_sciezka;
	string abc;
	vector <string> abcx;
	BazaInfryKolejowejFromFile bui(baza_sciezka);
	BazaInfryKolejowej* bazaa = new BazaInfryKolejowej(bui);
	vector <int> wynik;
	int x;
	for(int i=0; i<8; i++)
	{
		cin>>x;
		wynik.push_back(x);
	}
	cout<<"hutu.."<<endl;
	while(cin>>abc)
	{
		abcx.push_back(abc);
	}
	cout<<"tutsi.."<<endl;
	algopoc alg(8, 25, 5, 5, bazaa, abcx);
	cout<<"KONIEC ALGORYTMU.."<<endl;
	BazaRuchuKolejowego* bazab=alg.generuj(wynik);
	cout<<"KONIEC ALGORYTMU2.."<<endl;
	vector <Pociag*> pociagiW=bazab->getPociagi();
	cout<<"KONIEC ALGORYTMU3.."<<endl;
	int s1 = pociagiW.size();
	vector <string> pociagi;
	for(int i=0; i<s1; i++)
	{
		pociagi.push_back(pociagiW[i]->idPociagu);
		cout<<"GENERUJ..."<<i<<endl;
		generujpociag(i, bazaa, bazab);
	}
	/*
	for(int i=0; i<bazaa.liczba_stacji(); i++)
	{
		generujstacje(bazaa, &bazab, i);
	}
	*/

	for(int j=0; j<bazaa->wszystkieTory().size(); j++)
	{
		generujtorhtml(bazaa, bazab, j);
	}
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
//	}
//	vector <vector <string> > caboom=generujobiegi(bazaa, &bazab);
//	generujzestawienie(caboom, bazaa, bazab);
//	generuj_cegla2(bazaa, &bazab, pociagi, "data/cegla.html");
}
