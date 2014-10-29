#ifndef T6HPP
#define T6HPP
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "genalgpoc.hpp"
#include "pkpbaza.hpp"
#include "genalg.hpp"
#include <cmath>
using namespace std;


string inttostring(int alfa)
{
	stringstream tmp;
	tmp<<alfa;
	string wynik;
	tmp>>wynik;
	return wynik;
}


string ignoruj_podkreslniki(string data)
{
	int s1=data.length();
	for(int i=0; i<s1; i++)
	{
		if(data[i]=='_')
			data[i]=' ';
	}
	return data;
}

void generujpociag(int nkol, BazaInfryKolejowej* bazaa, BazaRuchuKolejowego* bazab)
{
	Pociag* foo=(bazab->getPociagi())[nkol];
	string str="data/pociag"+foo->idPociagu+".html";
	fstream plik(str.c_str(), fstream::trunc | fstream::out);
	plik<<"<!DOCTYPE html>\n<html lang=\"pl\">\n<head> <meta charset=\"utf8\"> </head>\n<body>\n";
	//plik<<"stracony czas (sekundy): "<<foo.stracony_czas<<"</br>\n";
	
	vector <PostojPociagu> postoje = foo->getPostojePociagu();

	int s1=postoje.size();
	for(int j=0; j<s1; j++)
	{
		cout<<"ok "<<j<<endl;
		PostojPociagu po = postoje[j];
		string a2;
		cout<<"TYP "<<po.typ<<endl;
		if(po.typ==0)
		{
			a2="<a href=\"stacja"+inttostring(po.stacja->getId())+".html\">"+ignoruj_podkreslniki(po.stacja->getNazwa())+"</a>";
cout<<po.stacja->getNazwa()<<" STANICA"<<endl;
}
		else
			a2="<a href=\"przystanek"+inttostring(po.przystanek->getId())+".html\">"+ignoruj_podkreslniki(po.przystanek->getNazwa())+"</a>";
		plik<<a2<<" ";
		plik<<totimesec(po.timePrzyjazd)<<" "<<totimesec(po.timeOdjazd)<<"</br>"<<endl;
	}
	plik<<"</body></html>";
	plik.close();
}


/*void generujzestawienie (vector <vector <string> > data, BazaInfryKolejowej* bazaa, BazaRuchuKolejowego* bazab)
{
	string str="data/zestawienie.html";
	fstream plik(str.c_str(), fstream::trunc | fstream::out);
	plik<<"<!DOCTYPE html>\n<html lang=\"pl\">\n<head> <meta charset=\"utf8\"> </head>\n<body><table>\n";
	int s1=data.size();
	vector <string> obiegi_linijki;
	for(int i=0; i<s1; i++)
	{
		int obieg_distance=0;
		int obieg_czas_calk;
		int obieg_czas_start;
		int obieg_czas_stop;
		int obieg_czas_jazd=0;
		int obieg_stracony=0;
		int obieg_stacja_koncowa;
		int obieg_stacja_poczatkowa;
		plik<<"<tr><td colspan=\"8\"><b>OBIEG "<<i<<"</b></br></td></tr>"<<endl;
		int s2=data[i].size();
		for(int j=0; j<s2; j++)
		{
			Pociag* akt=bazab->getPociag(data[i][j]);
			if(j==0)
			{
				obieg_stacja_poczatkowa=akt.stacja_poczatkowa();
				obieg_czas_calk=akt.czas_start();
				obieg_czas_start=akt.czas_start();
			}
			if(j==s2-1)
			{
				obieg_stacja_koncowa=akt.stacja_koncowa();
				obieg_czas_calk=akt.czas_stop()-obieg_czas_calk;
				obieg_czas_stop=akt.czas_stop();
			}
			obieg_czas_jazd+=akt.czas_przejazdu();
			obieg_distance+=akt.distance;
			obieg_stracony+=akt.stracony_czas;
			plik<<"<tr>";
			string a1="<td><a href=\"pociag"+data[i][j]+".html\">"+data[i][j]+"</a></td><td>"+inttostring(akt.stracony_czas)+"<td>";
			string a2="<td>"+inttostring(akt.czas_przejazdu()/60)+"</td>";
			string a21="<td>"+inttostring(akt.distance)+"</td>";
			string a33="<a href=\"stacja"+inttostring(akt.stacja_poczatkowa())+".html\">"+ignoruj_podkreslniki(bazaa.nazwa_stacji(akt.stacja_poczatkowa()))+"</a>";
			string a34=totimemin(akt.czas_start());
			string a44=totimemin(akt.czas_stop());
			string a43="<a href=\"stacja"+inttostring(akt.stacja_koncowa())+".html\">"+ignoruj_podkreslniki(bazaa.nazwa_stacji(akt.stacja_koncowa()))+"</a>";
			string a3="<td>"+a33+"</td><td>"+a34+"</td>";
			string a4="<td>"+a43+"</td><td>"+a44+"</td>";
			plik<<a1<<a2<<a21<<a3<<a4<<endl;
			plik<<"</tr>";
		}
		string a2="<td>"+inttostring(obieg_czas_calk/60)+"</td>";
		string a21="<td>"+inttostring(obieg_czas_jazd/60)+"</td>";
		string a22="<td>"+inttostring(obieg_stracony/60)+"</td>";
		string a23="<td>"+inttostring(obieg_distance)+"</td>";
		string a24="<td><a href=\"stacja"+inttostring(obieg_stacja_poczatkowa)+".html\">"+ignoruj_podkreslniki(bazaa.nazwa_stacji(obieg_stacja_poczatkowa))+"</a></td>";
		string a25="<td><a href=\"stacja"+inttostring(obieg_stacja_koncowa)+".html\">"+ignoruj_podkreslniki(bazaa.nazwa_stacji(obieg_stacja_koncowa))+"</a></td>";
		string a26="<td>"+totimemin(obieg_czas_start)+"</td>";
		string a27="<td>"+totimemin(obieg_czas_stop)+"</td>";
		obiegi_linijki.push_back("<tr>"+a2+a21+a22+a23+a24+a26+a25+a27+"</tr>");
	}
	plik<<"</table><table>"<<endl;
	for(int i=0; i<s1; i++)
	{
		plik<<obiegi_linijki[i]<<endl;
	}
	plik<<"</table></body></html>";
	plik.close();
}*/
/*
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
*/
/*
vector <pair <int, string> > pociagi_poczatkowe(int id, baza_pociagow* b2)
{
	vector <pair <int, string> > wynik;
	set <string> s1=b2->lista_pociagow_stacja(id);
	set <string>::iterator it1=s1.begin();
	while(it1!=s1.end())
	{
		string teraz=*it1;
		pociag aktualny=b2->poc(teraz);
		if(aktualny.punkty[0].typ==0 && aktualny.punkty[0].id_infr==id)
			wynik.push_back(pair<int, string>(aktualny.punkty[0].czas_odjazdu, teraz));
		it1++;
	}
	sort(wynik.begin(), wynik.end());
	return wynik;
}

vector <pair <int, string> > pociagi_koncowe(int id, baza_pociagow* b2)
{
	vector <pair <int, string> > wynik;
	set <string> s1=b2->lista_pociagow_stacja(id);
	set <string>::iterator it1=s1.begin();
	while(it1!=s1.end())
	{
		string teraz=*it1;
		pociag aktualny=b2->poc(teraz);
		int s3=aktualny.punkty.size();
		if(aktualny.punkty[s3-1].typ==0 && aktualny.punkty[s3-1].id_infr==id)
			wynik.push_back(pair<int, string>(aktualny.punkty[s3-1].czas_przyjazdu, teraz));
		it1++;
	}
	sort(wynik.begin(), wynik.end());
	return wynik;
}

pair <string, string> odszukaj_najblizsze(map <string, int> kon, map <string, int> poc)
{
	pair <string, string> wynik("", "");
	int minwynik=2000;
	map <string, int>::iterator it1=kon.begin();
	int licznik=0;
	while(it1!=kon.end())
	{
		map <string, int>::iterator it2=poc.begin();
		while(it2!=poc.end())
		{
			int konakt=it1->second/60;
			int pocakt=it2->second/60;
			if((pocakt-konakt)>=3)
			{
				if((pocakt-konakt)<minwynik)
				{
					minwynik=pocakt-konakt;
					wynik=pair <string, string>(it1->first, it2->first);
				}
			}
			it2++;
		}
		licznik++;
		it1++;
	}
	return wynik;
}

vector <vector <string> > generujobiegi(baza_sieci& bazaa, baza_pociagow* bazab)
{
	vector <vector <string> > wynik;
	map<string, string> nastepny;
	map<string, string> poprzedni;
	int s1=bazaa.liczba_stacji();
	for(int i=0; i<s1; i++)
	{
		vector <pair <int, string> > pkon=pociagi_koncowe(i, bazab);
		vector <pair <int, string> > ppoc=pociagi_poczatkowe(i, bazab);
		map<string, int> pp;
		map<string, int> pk;
		int k11=pkon.size();
		for(int j=0; j<k11; j++)
		{
			nastepny[pkon[j].second]="";
			pk[pkon[j].second]=pkon[j].first;
		}
		k11=ppoc.size();
		for(int j=0; j<k11; j++)
		{
			poprzedni[ppoc[j].second]="";
			pp[ppoc[j].second]=ppoc[j].first;
		}
		pair <string, string> naj=odszukaj_najblizsze(pk, pp);
		int licznik=0;
		while(naj.first!="")
		{
			licznik++;
			pk.erase(pk.find(naj.first));
			pp.erase(pp.find(naj.second));
			nastepny[naj.first]=naj.second;
			poprzedni[naj.second]=naj.first;
			naj=odszukaj_najblizsze(pk, pp);
		}
	}
	map <string, string>::iterator it1=poprzedni.begin();
	while(it1!=poprzedni.end())
	{
		if(it1->second=="")
		{
			vector <string> obieg;
			string aktu=it1->first;
			while(aktu!="")
			{
				obieg.push_back(aktu);
				aktu=nastepny[aktu];
			}
			wynik.push_back(obieg);
		}
		it1++;
	}
	return wynik;
}
void generujstacje (baza_sieci& bazaa, baza_pociagow* bazab, int i)
{
	string str="data/stacja"+inttostring(i)+".html";
	fstream plik(str.c_str(), fstream::trunc | fstream::out);
	plik<<"<!DOCTYPE html>\n<html lang=\"pl\">\n<head> <meta charset=\"utf8\"> </head>\n<body>\n";
	plik<<"STACJA: "<<bazaa.nazwa_stacji(i)<<"</br>"<<endl;
	set <int> pot=bazaa.powiazane_tory(i, 1);
	set <int>::iterator it1=pot.begin();
	vector <pair <int, string> > pkon=pociagi_koncowe(i, bazab);
	vector <pair <int, string> > ppoc=pociagi_poczatkowe(i, bazab);
	while(it1!=pot.end())
	{
		string a2="<a href=\"linia"+inttostring(*it1)+".html\">"+ignoruj_podkreslniki(bazaa.nazwa_stacji(bazaa.numer_stacji_pocz(*it1)))+" - "+ignoruj_podkreslniki(bazaa.nazwa_stacji(bazaa.numer_stacji_konc(*it1)))+"</a>";
		plik<<a2<<"</br>"<<endl;
		it1++;
	}
	for(int j=0; j<bazaa.liczba_stacji(); j++)
	{
		if(i!=j)
		{
			vector <pair <int, string> > wynik1=tablica_odjazdow(i, j, bazab);
			vector <vector <pair <int, string> > > godzinowe(24);
			int s2=wynik1.size();
			for(int g=0; g<s2; g++)
			{
				godzinowe[((wynik1[g].first)/60/60)%24].push_back(wynik1[g]);
			}
			if(wynik1.size()>0)
			{
				string a2="<a href=\"stacja"+inttostring(j)+".html\">"+ignoruj_podkreslniki(bazaa.nazwa_stacji(j))+"</a>";
				plik<<a2<<"</br>";
				for(int h=0; h<24; h++)
				{
					int s2=godzinowe[h].size();
					if(s2>0)
					{
						for(int g=0; g<s2; g++)
						{
							string txt1=totimemin(godzinowe[h][g].first);
							string a1="<a href=\"pociag"+godzinowe[h][g].second+".html\">"+txt1+"</a>";
							plik<<a1<<" ";
						}
						plik<<"</br>"<<endl;
					}
				}
				plik<<"</br>"<<endl;
			}
		}
	}
	int s17=pkon.size();
	for(int j=0; j<s17; j++)
	{
		plik<<pkon[j].second<<" "<<totimemin(pkon[j].first)<<"</br>"<<endl;
	}
	s17=ppoc.size();
	plik<<"</br>";
	for(int j=0; j<s17; j++)
	{
		plik<<ppoc[j].second<<" "<<totimemin(ppoc[j].first)<<"</br>"<<endl;
	}
	plik<<"</body>\n";
	plik<<"</html>";
	plik.close();
}
*/
void generujtorhtml (BazaInfryKolejowej* bazaa, BazaRuchuKolejowego* bazab, int nkol)
{
	string str="data/linia"+inttostring(nkol)+".html";
	fstream plik(str.c_str(), fstream::trunc | fstream::out);
	vector <TorSzlakowy*> torySzlakowe = bazaa->wszystkieTory();
	int stacja_pocz=torySzlakowe[nkol]->getStacjaPoczatkowa()->getId();
	int stacja_konc=torySzlakowe[nkol]->getStacjaKoncowa()->getId();
	plik<<"<b>"<<torySzlakowe[nkol]->getStacjaPoczatkowa()->getNazwa()<<"</b>"<<endl;
	plik<<"</br><b>"<<torySzlakowe[nkol]->getStacjaKoncowa()->getNazwa()<<"</b>"<<endl;
	RuchPociagowPoTorzeSzlakowym* ruch = bazab->torToRuch[torySzlakowe[nkol]];
	for(int i=0; i<ruch->ruchKierunekNormalny.size(); i++)
	{
		plik<<"<i>";
		plik<<ruch->ruchKierunekNormalny[i]->pociag->idPociagu<<" - "<<endl;
		plik<<totimesec(ruch->ruchKierunekNormalny[i]->czasWjazduNaTor)<<" - "<<endl;
		plik<<totimesec(ruch->ruchKierunekNormalny[i]->czasWyjazduZToru)<<";</br>"<<endl;
		plik<<"</i>";
	}
	for(int i=0; i<ruch->ruchKierunekPrzeciwny.size(); i++)
	{
		plik<<ruch->ruchKierunekPrzeciwny[i]->pociag->idPociagu<<" - "<<endl;
		plik<<totimesec(ruch->ruchKierunekPrzeciwny[i]->czasWjazduNaTor)<<" - "<<endl;
		plik<<totimesec(ruch->ruchKierunekPrzeciwny[i]->czasWyjazduZToru)<<";</br>"<<endl;
	}
	plik.close();
	/*vector <pair<pair <int, bool>, string> > czasy;
	vector <pociag_na_torze> alfa1=bazab->lista_toru_nor(nkol);
	vector <pociag_na_torze> alfa2=bazab->lista_toru_prz(nkol);
	int s1=alfa1.size();
	int s2=alfa2.size();
	for(int i=0; i<s1; i++)
	{
		czasy.push_back(pair<pair<int, bool>, string>(pair<int, bool>(alfa1[i].time_start, 0), alfa1[i].id));
	}
	for(int i=0; i<s2; i++)
	{
		czasy.push_back(pair<pair<int, bool>, string>(pair<int, bool>(alfa2[i].time_start, 1), alfa2[i].id));
	}
	int s4=czasy.size();
	sort(czasy.begin(), czasy.end());
	vector <vector <string> > tabelka;
	vector <string> lewakolumna(2);
	vector <pair <int, int> > przyst=bazaa.przystanki(nkol);
	string a1="<a href=\"stacja"+inttostring(stacja_pocz)+".html\">"+ignoruj_podkreslniki(bazaa.nazwa_stacji(stacja_pocz))+"</a>";
	string a2="<a href=\"stacja"+inttostring(stacja_konc)+".html\">"+ignoruj_podkreslniki(bazaa.nazwa_stacji(stacja_konc))+"</a>";
	lewakolumna.push_back(a1);
	int s99=przyst.size();
	for(int i=0; i<s99; i++)
	{
		//TODO EDIT
		string a2="<a href=\"przystanek"+inttostring(przyst[i].second)+".html\">"+ignoruj_podkreslniki(bazaa.nazwa_przystanku(przyst[i].second))+"</a>";
		lewakolumna.push_back(a2);
	}
	lewakolumna.push_back(a2);
	lewakolumna.push_back("");
	//tabelka.push_back(lewakolumna);
	for(int i=0; i<s4; i++)
	{
		pociag teraz=bazab->poc(czasy[i].second);
		vector <string> tabelka1;
		string c5="<a href=\"pociag"+czasy[i].second+".html\">"+czasy[i].second+"</a>";
		tabelka1.push_back(c5);
		string c1="<a href=\"stacja"+inttostring(teraz.stacja_poczatkowa())+".html\">"+ignoruj_podkreslniki(bazaa.nazwa_stacji(teraz.stacja_poczatkowa()))+"</a>";
		tabelka1.push_back(c1);
		bool czyprzec=czasy[i].first.second;
		if(!czyprzec)
		{
			int s7=teraz.punkty.size();
			int pkt_kol=0;
			while(teraz.punkty[pkt_kol].id_infr!=stacja_pocz || teraz.punkty[pkt_kol].typ!=0)
			{
				pkt_kol++;
			}
			string galfa=totimemin(teraz.punkty[pkt_kol].czas_odjazdu);
			if(teraz.punkty[pkt_kol].czas_odjazdu==teraz.punkty[pkt_kol].czas_przyjazdu)
				galfa="<i>"+galfa+"</i>";
			tabelka1.push_back(galfa);
			pkt_kol++;
			while(teraz.punkty[pkt_kol].id_infr!=stacja_konc || teraz.punkty[pkt_kol].typ!=0)
			{
				string g1=totimemin(teraz.punkty[pkt_kol].czas_odjazdu);
				if(teraz.punkty[pkt_kol].czas_odjazdu==teraz.punkty[pkt_kol].czas_przyjazdu)
					g1="<i>"+g1+"</i>";
				tabelka1.push_back(g1);
				pkt_kol++;
			}
			galfa=totimemin(teraz.punkty[pkt_kol].czas_przyjazdu);
			if(teraz.punkty[pkt_kol].czas_odjazdu==teraz.punkty[pkt_kol].czas_przyjazdu)
				galfa="<i>"+galfa+"</i>";
			tabelka1.push_back(galfa);
		}
		else
		{
		}
		string c2="<a href=\"stacja"+inttostring(teraz.stacja_koncowa())+".html\">"+ignoruj_podkreslniki(bazaa.nazwa_stacji(teraz.stacja_koncowa()))+"</a>";
		tabelka1.push_back(c2);
		tabelka.push_back(tabelka1);
	}
	plik<<"<!DOCTYPE html>\n<html lang=\"pl\">\n<head> <meta charset=\"utf8\"> </head>\n<body>\n";
	plik<<"<a href=\"stacja"<<stacja_pocz<<".html\">"<<ignoruj_podkreslniki(bazaa.nazwa_stacji(stacja_pocz))<<"</a>"<<"-";
	plik<<"<a href=\"stacja"<<stacja_konc<<".html\">"<<ignoruj_podkreslniki(bazaa.nazwa_stacji(stacja_konc))<<"</a>"<<"</br>\n";
	int s5=lewakolumna.size();
	int s6=tabelka.size();
	for(int g=0; g<(((s6-1)/10)+1); g++)
	{
		plik<<"<table>";
		for(int i=0; i<s5; i++)
		{
			plik<<"<tr>";
			plik<<"<td>"<<lewakolumna[i]<<"</td>"<<endl;
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
	plik.close();*/
}
#endif
