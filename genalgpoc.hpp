#ifndef GENALGPOCHPP
#define GENALGPOCHPP
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "pkpbaza.hpp"
#include "bazainfry.hpp"
#include "genalg.hpp"
#include "PreferowaneLinie.hpp"
#include <cmath>
struct kand_pociag_roz
{
	vector <pair <int, int> > pdata; // ID LICZBY LOSOWEJ, MODULO LICZBY
	int dodsuma; //DODATKOWA LICZBA DO CZASU
	kand_pociag kand;
	int stringtoint(string cos)
	{
		stringstream foo;
		foo<<cos;
		int wynik;
		foo>>wynik;
		return wynik;
	}
	kand_pociag_roz(stringstream& alpha, int kod, PreferowaneLinie& pref)
	{
		char a0[10000];
		string from;
		string to;
		alpha>>from;
		alpha>>to;
		alpha>>dodsuma;
		stringstream kodall;
		kodall<<pref.getKod1()<<kod<<"_"<<pref.getKod2();
		kodall>>kand.kod;
		kand.przyspieszenie=pref.getPrzyspieszenie();
		kand.vmaxkph=pref.getvmax();
		kand.czas_postoju_przystanek=pref.getCzasPostojuPrzystanek();
		kand.stacja_srodkowa=pref.getStacjaSrodkowa();
		kand.tory_przejazdowe=pref.toryPosrednie(from, to);
		for(int i=0; i<kand.tory_przejazdowe.size(); i++)
		{
			cout<<"tory"<<kand.tory_przejazdowe[i]<<endl;
		}
		kand.przystanki_postojowe=pref.przystankiPosrednie(from, to);
		kand.stacje_postojowe=pref.stacjePosrednie(from, to);
		for(int i=0; i<kand.stacje_postojowe.size(); i++)
		{
			cout<<"stacje"<<kand.stacje_postojowe[i].first<<" "<<kand.stacje_postojowe[i].second<<endl;
		}
		cout<<kand.kod<<" "<<dodsuma<<endl;
		string teraz;
		alpha>>teraz;
		while(teraz[0]!='#')
		{
			int k1=stringtoint(teraz);
			int k2;
			alpha>>k2;
			pdata.push_back(pair<int, int>(k1, k2));
			alpha>>teraz;
		}
		cout<<"kuniec"<<endl;
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
	BazaInfryKolejowej* baz;
	vector <string> sciezki;
	vector <kand_pociag_roz> kandydaci;
	PreferowaneLinie aktConfig;
	int ocen(vector <int> przyklad)
	{
		for(int i=0; i<przyklad.size(); i++)
		{
			cout<<przyklad[i]<<"; ";
		}
		cout<<endl;
		int wynik=0;
		int s1=kandydaci.size();
		vector <kand_pociag> kandydaci2;
		for(int i=0; i<s1; i++)
		{
			kand_pociag kanx=kandydaci[i].generuj(przyklad);
			kandydaci2.push_back(kanx);
		}
		BazaRuchuKolejowego* przykladowa=new BazaRuchuKolejowego(kandydaci2, baz);
		vector <Pociag*> pociagi=przykladowa->getPociagi();
		for(int i=0; i<s1; i++)
		{
			cout<<pociagi[i]->getCzasStart()<<", ";
			wynik+=pociagi[i]->czasPrzejazdu();
		}
		cout<<endl;
		delete przykladowa;
		cout<<"OCENA "<<wynik/60<<endl;
		return 2000000-wynik/500;
	}
	public:
	BazaRuchuKolejowego* generuj(vector <int> lista)
	{
		int s1=kandydaci.size();
		vector <kand_pociag> kandydaci2;
		for(int i=0; i<s1; i++)
		{
			kand_pociag kanx=kandydaci[i].generuj(lista);
			kandydaci2.push_back(kanx);
		}
		BazaRuchuKolejowego* wynik=new BazaRuchuKolejowego(kandydaci2, baz);
		return wynik;
	}
	algopoc(int liczba_genow, int liczba_osobnikow, int ile_wywalic, int do_mutacji, BazaInfryKolejowej* baza, vector <string> pocdata) : genalgo(liczba_genow, liczba_osobnikow, ile_wywalic, do_mutacji), baz(baza), sciezki(pocdata)
	{
		int s9=pocdata.size();
		for(int g=0; g<s9; g++)
		{
			fstream plik;
			plik.open(pocdata[g].c_str());
			int i=0;
			while(!plik.eof())
			{
				char data[10000];
				plik.getline(data, 10000);
				stringstream olx;
				olx<<data;
				string first;
				olx>>first;
				cout<<first<<endl;
				if(first=="@")
					aktConfig=PreferowaneLinie(baza, olx);
				if(first=="#")
				{
					kand_pociag_roz nowy(olx, i, aktConfig);
					kandydaci.push_back(nowy);
					i++;
				}
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
};
#endif
