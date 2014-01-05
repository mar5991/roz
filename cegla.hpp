#ifndef CEGLAHPP
#define CEGLAHPP
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "pkpbaza.hpp"
#include "genalg.hpp"
#include <cmath>
#include "t6.hpp"
struct kand_stacja_cegla
{
	int id_infr;
	bool typ; // 0=stacja, 1=przystanek
	bool czy_odjazd;
};

int wyszukajcegla(vector <punkt_posr> po, kand_stacja_cegla cegla)
{
	int wynik=-1;
	int s1=po.size();
	for(int i=0; i<s1; i++)
	{
		if(cegla.id_infr==po[i].id_infr && cegla.typ==po[i].typ)
			return i;
	}
	return wynik;
}

vector <string> generuj_cegla1(baza_sieci& bazaa, pociag p1, vector <kand_stacja_cegla> cegla)
{
	int stacja_poczatkowa=p1.stacja_poczatkowa();
	int stacja_koncowa=p1.stacja_koncowa();
	int pocz_alfa=-1;
	int konc_alfa=-1;
	bool znaleziono_pocz=0;
	bool znaleziono_konc=0;
	int s1=cegla.size();
	for(int i=0; i<s1 && !znaleziono_pocz; i++)
	{
		if(cegla[i].id_infr==stacja_poczatkowa && cegla[i].typ==0)
		{
			pocz_alfa=i;
			znaleziono_pocz=1;
		}
	}
	for(int i=s1-1; i>=0 && !znaleziono_konc; i--)
	{
		if(cegla[i].id_infr==stacja_koncowa && cegla[i].typ==0)
		{
			konc_alfa=i;
			znaleziono_konc=1;
		}
	}
	vector <string> wynik(s1+3);
	for(int i=0; i<s1+3; i++)
	{
		wynik[i]=".";
	}
	wynik[0]=p1.kod;
	if(pocz_alfa==-1)
	{
		wynik[1]=ignoruj_podkreslniki(bazaa.nazwa_stacji(stacja_poczatkowa));
	}
	if(konc_alfa==-1)
	{

		wynik[s1+2]=ignoruj_podkreslniki(bazaa.nazwa_stacji(stacja_koncowa));
	}
	if(pocz_alfa==-1)
		pocz_alfa=0;
	if(konc_alfa==-1)
		konc_alfa=s1-1;
	for(int i=pocz_alfa; i<=konc_alfa; i++)
	{
		int ag1=wyszukajcegla(p1.punkty, cegla[i]);
		if(ag1==-1)
		{
			wynik[i+2]==">";
		}
		else
		{
			int cc1=p1.punkty[ag1].czas_odjazdu;
			if(cegla[i].czy_odjazd==0)
				cc1=p1.punkty[ag1].czas_przyjazdu;
			wynik[i+2]=totimemin(cc1);
		}
	}
	return wynik;
}

void generuj_cegla2(baza_sieci& bazaa, baza_pociagow* bazab, vector<string> pociagi, string output)
{
	vector <kand_stacja_cegla> cegla;
	srand(time(NULL));
	pociag genesis=bazab->poc(pociagi[rand()%pociagi.size()]);
	vector <punkt_posr> pg=genesis.punkty;
	int s1=pg.size();
	cout<<s1<<"pg"<<endl;
	for(int i=0; i<s1; i++)
	{
		kand_stacja_cegla k1;
		k1.id_infr=pg[i].id_infr;
		k1.typ=pg[i].typ;
		k1.czy_odjazd=true;
		cegla.push_back(k1);
	}
	vector <string> naglowki(2);
	int s3=cegla.size();
	for(int i=0; i<s3; i++)
	{
		string literki=" p";
			if(cegla[i].czy_odjazd)
				literki=" o";
		if(cegla[i].typ==0)
		{
			naglowki.push_back(ignoruj_podkreslniki(bazaa.nazwa_stacji(cegla[i].id_infr))+literki);
		}
		else
		{
			naglowki.push_back(ignoruj_podkreslniki(bazaa.nazwa_przystanku(cegla[i].id_infr))+literki);
		}
	}
	naglowki.push_back("");
	vector <vector <string> > tabelka;
	int s2=pociagi.size();
	for(int i=0; i<s2; i++)
	{
		tabelka.push_back(generuj_cegla1(bazaa, bazab->poc(pociagi[i]), cegla));
	}
	string str=output;
	fstream plik(str.c_str(), fstream::trunc | fstream::out);
	plik<<"<!DOCTYPE html>\n<html lang=\"pl\">\n<head> <meta charset=\"utf8\"> </head>\n<body>\n";
	int s5=tabelka[0].size();
	int s6=tabelka.size();
	int i=0;
	for(int g=0; g<(((s6+1)/10)+1); g++)
	{
		plik<<"<table>";
		for(int i=0; i<s5; i++)
		{
			plik<<"<tr>";
			plik<<"<td>"<<naglowki[i]<<"</td>";
			for(int j=g*10; j<(g*10+10) && j<s6; j++)
			{
				plik<<"<td>";
				if(tabelka[j].size()>i)
					plik<<tabelka[j][i];
				plik<<"</td>";
			}
			plik<<"</tr>\n";
		}
		plik<<"</table></br>";
	}
	plik<<"</body>\n";
	plik<<"</html>";
	plik.close();
	cout<<"ckd"<<endl;
}

#endif
