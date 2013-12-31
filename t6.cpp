#include <iostream>
#include <sstream>
#include "pkpbaza.hpp"
#include "genalg.hpp"
using namespace std;

vector <pair <string, int> > polaczenia(int start, bool start_type, int stop, bool stop_type)
{
	//TODO
}

struct kand_pociag_roz
{
	vector <pair <int, int> > pdata; // ID LICZBY LOSOWEJ, MODULO LICZBY
	int dodsuma; //DODATKOWA LICZBA DO CZASU
	kand_pociag kand;
	kand_pociag_roz(fstream& alpha)
	{
		dodsuma=0;
		char a0[10000];
		char a1[10000];
		char a2[10000];
		char a3[10000];
		alpha.getline(a0, 10000);
		alpha.getline(a1, 10000);
		alpha.getline(a2, 10000);
		alpha.getline(a3, 10000);
		stringstream b0, b1, b2, b3;
		b0<<a0;
		b1<<a1;
		b2<<a2;
		b3<<a3;
		b0>>kand.kod;
		b0>>kand.przyspieszenie;
		b0>>kand.vmaxkph;
		b0>>kand.czas_postoju_przystanek;
		int k1;
		while(b1>>k1)
		{
			int k2;
			b1>>k2;
			pdata.push_back(pair<int, int>(k1, k2));
		}
		while(b2>>k1)
		{
			dodsuma+=k1;
		}
		string k3;
		vector <string> gamma;
		while(b3>>k3)
		{
			gamma.push_back(k3);
		}
		kand.wypelnijtrase(gamma);
	}
	kand_pociag generuj(vector <int> losowe)
	{
		int dataroz=dodsuma*60;
		int s1=pdata.size();
		for(int i=0; i<s1; i++)
		{
			dataroz+=(losowe[pdata[i].first]%pdata[i].second)*60;
		}
		kand.time_start=dataroz;
		return kand;
	}
};
class algopoc : genalgo
{
	baza_sieci* baz;
	vector <kand_pociag_roz> kandydaci;
	int ocen(vector <int> przyklad)
	{
		int wynik=0;
		baza_pociagow przykladowa=baz->generuj_baze();
		int s1=kandydaci.size();
		for(int i=0; i<s1; i++)
		{
			kand_pociag kanx=kandydaci[i].generuj(przyklad);
			przykladowa.dodaj_pociag(kanx);
		}
		for(int i=0; i<s1; i++)
		{
			wynik+=przykladowa.poc(i).czas_przejazdu();
		}
		cout<<"OCENA "<<wynik/60<<endl;
		return 2000000-wynik/500;
	}
	public:
	baza_pociagow generuj(vector <int> lista)
	{
		baza_pociagow wynik=baz->generuj_baze();
		int s1=kandydaci.size();
		for(int i=0; i<s1; i++)
		{
			kand_pociag kanx=kandydaci[i].generuj(lista);
			wynik.dodaj_pociag(kanx);
		}
		return wynik;
	}
	algopoc(int liczba_genow, int liczba_osobnikow, int ile_wywalic, int do_mutacji, baza_sieci* baza, string pocdata) : genalgo(liczba_genow, liczba_osobnikow, ile_wywalic, do_mutacji), baz(baza)
	{
		fstream plik;
		plik.open(pocdata.c_str());
		char t1[10000];
		plik.getline(t1, 10000);
		stringstream tp1;
		tp1<<t1;
		int tp2;
		tp1>>tp2;
		for(int i=0; i<tp2; i++)
		{
			kand_pociag_roz nowy(plik);
			kandydaci.push_back(nowy);
		}
	}
	vector <int> get_wynik(int lpowtorzen)
	{
		vector <int> thebest;
		int ocenamax=0;
		for(int i=0; i<lpowtorzen; i++)
		{
			int tmpwynik;
			vector <int> kabc=losuj(tmpwynik);
			if(tmpwynik>ocenamax)
			{
				thebest=kabc;
			}
		}
		return thebest;
	}
};

void wypiszpociag(int nkol, baza_pociagow& bazab)
{
	pociag foo=bazab.poc(nkol);
	cout<<foo.kod<<endl;
	int s1=foo.punkty.size();
	for(int j=0; j<s1; j++)
	{
		punkt_posr po=foo.punkty[j];
		cout<<bazab.nazwa_postojowa(foo.kod, j)<<" "<<totimesec(po.czas_przyjazdu)<<" "<<totimesec(po.czas_odjazdu)<<endl;
	}
}

vector <pair <int, string> > tablica_odjazdow(int id1, int id2, baza_pociagow* b2) //para czas odjazdu, id_pociagu
{
	vector <pair <int, string> > wynik;
	set <string> s1=b2->lista_pociagow_stacja(id1);
	set <string>::iterator it1=s1.begin();
	while(it1!=s1.end())
	{
		string teraz=*it1;
		pociag aktualny=b2->poc(teraz);
		int s1=aktualny.punkty.size();
		bool juz=0;
		int kodgen;
		for(int i=0; i<s1; i++)
		{
			if(aktualny.punkty[i].typ==0 && aktualny.punkty[i].id_infr==id1 && aktualny.punkty[i].czas_odjazdu!=aktualny.punkty[i].czas_przyjazdu)
			{
				juz=1;
				kodgen=i;
			}
			if(aktualny.punkty[i].typ==0 && aktualny.punkty[i].id_infr==id2 && juz && aktualny.punkty[i].czas_odjazdu!=aktualny.punkty[i].czas_przyjazdu)
			{
				wynik.push_back(pair<int, string>(aktualny.punkty[kodgen].czas_odjazdu, teraz));
			}
		}
		it1++;
	}
	sort(wynik.begin(), wynik.end());
	return wynik;
}

int main()
{
	string baza_sciezka;
	cin>>baza_sciezka;
	baza_sieci bazaa(baza_sciezka);
	string abc;
	cin>>abc;
	algopoc alg(8, 25, 5, 5, &bazaa, abc);
	vector <int> wynik=alg.get_wynik(50);
	baza_pociagow bazab=alg.generuj(wynik);
	int as1=bazab.liczba_pociagow();
	for(int i=0; i<as1; i++)
	{
		wypiszpociag(i, bazab);
	}
	for(int i=0; i<28; i++)
	{
		cout<<"STACJA: "<<bazaa.nazwa_stacji(i)<<endl;
		for(int j=0; j<28; j++)
		{
			if(i!=j)
			{
				vector <pair <int, string> > wynik1=tablica_odjazdow(i, j, &bazab);
				if(wynik1.size()>0)
				{
					cout<<bazaa.nazwa_stacji(j)<<" ";
					int s2=wynik1.size();
					for(int g=0; g<s2; g++)
					{
						cout<<totimemin(wynik1[g].first)<<" ";
					}
					cout<<endl;
				}
			}
		}
	}
	for(int j=0; j<32; j++)
	{
		cout<<"TOR "<<bazaa.nazwa_stacji(bazaa.numer_stacji_pocz(j))<<" - "<<bazaa.nazwa_stacji(bazaa.numer_stacji_konc(j))<<endl;
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
		}
	}
}
