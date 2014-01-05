#ifndef GENALGPOCHPP
#define GENALGPOCHPP
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "pkpbaza.hpp"
#include "genalg.hpp"
#include <cmath>
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
		b0>>kand.stacja_srodkowa;
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
	kand_pociag generuj()
	{
		int dataroz=dodsuma*60;
		kand.time_start=dataroz;
		return kand;
	}
};
class algopoc : genalgo
{
	baza_sieci* baz;
	vector <string> sciezki;
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
	algopoc(int liczba_genow, int liczba_osobnikow, int ile_wywalic, int do_mutacji, baza_sieci* baza, vector <string> pocdata) : genalgo(liczba_genow, liczba_osobnikow, ile_wywalic, do_mutacji), baz(baza), sciezki(pocdata)
	{
		int s9=pocdata.size();
		for(int g=0; g<s9; g++)
		{
			fstream plik;
			plik.open(pocdata[g].c_str());
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
	void generujprostrze(string prefix, int lpowtorzen)
	{
		vector <int> gw=get_wynik(lpowtorzen);
		int s1=sciezki.size();
		for(int i=0; i<s1; i++)
		{
			fstream plik;
			string nowasciezka=prefix+sciezki[i];
			fstream output(nowasciezka.c_str(), fstream::trunc | fstream::out);
			plik.open(sciezki[i].c_str());
			char tmp1[10000];
			plik.getline(tmp1, 10000);
			stringstream iter1;
			iter1<<tmp1;
			int s2;
			iter1>>s2;
			output<<tmp1<<endl;
			for(int j=0; j<s2; j++)
			{
				char tmp2[10000];
				plik.getline(tmp2, 10000);
				char tmp3a[10000];
				plik.getline(tmp3a, 10000);
				char tmp3b[10000];
				plik.getline(tmp3b, 10000);
				output<<tmp2<<endl<<endl;
				stringstream data1, data2;
				data1<<tmp3a;
				data2<<tmp3b;
				int aktliczba;
				data2>>aktliczba;
				int ck1, ck2;
				while(data1>>ck1)
				{
					data1>>ck2;
					aktliczba+=gw[ck1]%ck2;
				}
				output<<aktliczba<<endl;
				char tmp4[10000];
				plik.getline(tmp4, 10000);
				output<<tmp4<<endl;
			}
			plik.close();
			output.close();
		}
	}
};


baza_pociagow utworzbaze (baza_sieci* baza, vector <string> pocdata)
{
	vector <kand_pociag_roz> kandydaci;
	int s9=pocdata.size();
	for(int g=0; g<s9; g++)
	{
		fstream plik;
		plik.open(pocdata[g].c_str());
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
	baza_pociagow wynik=baza->generuj_baze();
	int s1=kandydaci.size();
	for(int i=0; i<s1; i++)
	{
		kand_pociag kanx=kandydaci[i].generuj();
		wynik.dodaj_pociag(kanx);
	}
	return wynik;

}
#endif
