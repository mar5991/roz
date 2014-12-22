#ifndef T6HPP
#define T6HPP
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "genalgpoc.hpp"
#include "pkpbaza.hpp"
#include "genalg.hpp"
#include "BazaObiegow.hpp"
#include "LiczbaPociagowStacja.hpp"
#include "plot.hpp"
#include <cmath>
using namespace std;

string textPoc()
{

string str1="<div style=\"position: relative;\"><canvas id=\"mycanvas\" width=\"1200\" height=\"1200\" style=\"position: absolute; left: 0; top: 0; z-index: 0;\"></canvas>";
string str2=" <canvas id=\"canvas2\" width=\"400\" height=\"400\"  style=\"position: absolute; left: 0; top: 0; z-index: 1;\"></canvas>";
string str3=" <canvas id=\"canvas3\" width=\"400\" height=\"400\"  style=\"position: absolute; left: 0; top: 0; z-index: -2;\"></canvas>";
string str4="<div><button id =\"kupa\" onclick=\"clearzoom()\" >Clear zoom</button></div>";
string str5="<script src=\"obr.js\"></script>";
return str1+str2+str3+str4+str5;
}

struct htmlgen
{
	static string zlozTabele(string tableclass, string lineclass, string cellclass, vector <vector <string> > tabela)
	{
		string wynik;
		int s1=tabela.size();
		for(int i=0; i<s1; i++)
		{
			string wynik0;
			for(int j=0; j<tabela[i].size(); j++)
			{
				wynik0+=div(cellclass, "", tabela[i][j]);
			}
			wynik+=div(lineclass, "", wynik0);
		}
		return div(tableclass, "", wynik);
	}
	static void zlozHtml(string content, string file_out, string css_file)
	{
		fstream plik(file_out.c_str(), fstream::trunc | fstream::out);
		string wynik;
		wynik+="<!DOCTYPE html>\n<html lang=\"pl\">\n<head> <meta charset=\"utf8\">";
		wynik+="<link rel=\"stylesheet\" type=\"text/css\" href=\""+css_file+"\">";
		wynik+="</head>\n<body>\n";
		wynik+=content+"\n";
		wynik+="</body></html>";
		plik<<wynik<<endl;
		plik.close();
	}
	static string div(string classname, string id, string info)
	{
		string wynik;
		wynik+="<div class=\"";
		wynik+=classname;
		wynik+="\" id=\"";
		wynik+=id;
		wynik+="\">";
		wynik+=info;
		wynik+="</div>\n";
		return wynik;
	}
	static string link(string link, string info)
	{
		string wynik;
		wynik+="<a href=\"";
		wynik+=link;
		wynik+="\">";
		wynik+=info;
		wynik+="</a>";
		return wynik;
	}
};

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

bool cmpVectors(vector <int> alfa, vector <int> beta)
{
	for(int i=0; i<min(alfa.size(), beta.size()); i++)
	{
		if(alfa[i]!=beta[i])
			return false;
	}
	return true;
}


string przystanekLink(PrzystanekKolejowy* przystanek)
{
	return htmlgen::link("przystanek"+inttostring(przystanek->getId())+".html", ignoruj_podkreslniki(przystanek->getNazwa()));
}
string stacjaLink(StacjaKolejowa* stacja)
{
	return htmlgen::link("stacja"+inttostring(stacja->getId())+".html", ignoruj_podkreslniki(stacja->getNazwa()));
}
string torLinkShort(TorSzlakowy* tor)
{
	return htmlgen::link("linia"+inttostring(tor->getId())+".html", "Tor "+inttostring(tor->getId()));
}
string torLinkLong(TorSzlakowy* tor)
{
	return htmlgen::link("linia"+inttostring(tor->getId())+".html", "Tor "+inttostring(tor->getId())+" "+ignoruj_podkreslniki(tor->getStacjaPoczatkowa()->getNazwa())+" - "+ignoruj_podkreslniki(tor->getStacjaKoncowa()->getNazwa()));
}
void generujpociag(int nkol, BazaInfryKolejowej* bazaa, BazaRuchuKolejowego* bazab)
{
	cout<<nkol<<endl;
	Pociag* foo=(bazab->getPociagi())[nkol];
	string str="data/pociag"+foo->getIdPociagu()+".html";
	string str2=foo->getIdPociagu()+".png";
	cout<<"okok"<<endl;
	stringstream plik;
	plik<<easyTrainPlot(foo);
	vector <PostojPociagu> postoje = foo->getPostojePociagu();
	int s1=postoje.size();
	for(int j=0; j<s1; j++)
	{
		PostojPociagu po = postoje[j];
		string a2;
		if(po.typ==0)
		{
			plik<<foo->czasPostojuPlanowego[po.stacja]<<" ";
			a2="<a href=\"stacja"+inttostring(po.stacja->getId())+".html\">"+ignoruj_podkreslniki(po.stacja->getNazwa())+"</a>";
}
		else
			a2="<a href=\"przystanek"+inttostring(po.przystanek->getId())+".html\">"+ignoruj_podkreslniki(po.przystanek->getNazwa())+"</a>";
		plik<<a2<<" ";
		plik<<totimesec(po.timePrzyjazd)<<" "<<totimesec(po.timeOdjazd)<<"</br>"<<endl;
	}
	plik<<"<img src=\""<<str2<<"\"/>"<<endl;
	plik<<textPoc();
	htmlgen::zlozHtml(plik.str(), str, "style.css");
	cout<<"FFFFF"<<endl;
}

vector <vector <int> > tabelaZajetosciTorowStacyjnych2(LiczbaPociagowStacja& lps, vector <string> typy)
{
	vector <vector <int> > wynik;
	int czasStartStan=0;
	vector <int> partialInfo(typy.size()*2+2);
	for(int i=0; i<lps.DLUGOSC_DNIA; i++)
	{
		vector <int> newPartialInfo;
		for(int j=0; j<typy.size(); j++)
		{
			newPartialInfo.push_back(lps.zajetoscTorowOdstawczych(typy[j], i));
		}
		newPartialInfo.push_back(lps.zajetoscTorowOdstawczych(i));
		for(int j=0; j<typy.size(); j++)
		{
			newPartialInfo.push_back(lps.zajetoscWszystkichTorow(typy[j], i));
		}
		newPartialInfo.push_back(lps.zajetoscWszystkichTorow(i));
		if(!cmpVectors(partialInfo, newPartialInfo))
		{
			if(i!=0)
			{
				vector <int> tmp;
				tmp.push_back(czasStartStan);
				tmp.push_back(i-1);
				tmp.insert(tmp.end(), partialInfo.begin(), partialInfo.end());
				wynik.push_back(tmp);
				czasStartStan=i;
			}
			partialInfo=newPartialInfo;
		}
	}
	vector <int> tmp;
	tmp.push_back(czasStartStan);
	tmp.push_back(lps.DLUGOSC_DNIA-1);
	tmp.insert(tmp.end(), partialInfo.begin(), partialInfo.end());
	wynik.push_back(tmp);
	return wynik;
}
string tabelaZajetosciTorowStacyjnych(LiczbaPociagowStacja& lps, vector <string> typy)
{
	string wynik;
	vector <vector <int> > praLista=tabelaZajetosciTorowStacyjnych2(lps, typy);
	string wynikLine0;
	wynikLine0+=htmlgen::div("tabelazajetosci_lead", "", "");
	wynikLine0+=htmlgen::div("tabelazajetosci_lead", "", "");
	for(int j=0; j<typy.size(); j++)
	{
		wynikLine0+=htmlgen::div("tabelazajetosci_lead", "", typy[j]);
	}
	wynikLine0+=htmlgen::div("tabelazajetosci_lead", "", inttostring(lps.maksymalnaZajetoscOdstawczych()));
	for(int j=0; j<typy.size(); j++)
	{
		wynikLine0+=htmlgen::div("tabelazajetosci_lead", "", typy[j]);
	}
	wynikLine0+=htmlgen::div("tabelazajetosci_lead", "", inttostring(lps.maksymalnaZajetoscWszystkich()));
	wynik+=htmlgen::div("tabelazajetosci_linelead", "", wynikLine0);
	for(int i=0; i<praLista.size(); i++)
	{
		string wynikLine;
		wynikLine+=htmlgen::div("tabelazajetosci_time1", "", totimesec(praLista[i][0]));
		wynikLine+=htmlgen::div("tabelazajetosci_time1", "", totimesec(praLista[i][1]));
		for(int j=0; j<typy.size(); j++)
		{
			wynikLine+=htmlgen::div("tabelazajetosci_l1", "", inttostring(praLista[i][j+2]));
		}
			wynikLine+=htmlgen::div("tabelazajetosci_l2", "", inttostring(praLista[i][typy.size()+2]));
		for(int j=0; j<typy.size(); j++)
		{
			wynikLine+=htmlgen::div("tabelazajetosci_l1", "", inttostring(praLista[i][typy.size()+j+3]));
		}
			wynikLine+=htmlgen::div("tabelazajetosci_l2", "", inttostring(praLista[i][typy.size()*2+3]));
		wynik+=htmlgen::div("tabelazajetosci_line", "", wynikLine);
	}
	wynik=htmlgen::div("tabelazajetosci", "", wynik);
	return wynik;
}

void generujstacje(int nkol, BazaInfryKolejowej* bazaa, BazaRuchuKolejowego* bazab, BazaObiegow& bazac)
{
	string str="data/stacja"+inttostring(nkol)+".html";
	StacjaKolejowa* stanica = bazaa->wszystkieStacje()[nkol];
	LiczbaPociagowStacja liczbaPoc(bazac, stanica, bazaa, bazab);
	RuchPociagowPrzezStacje* ruch = bazab->stacjaToRuch[stanica];
	string acc_all;
	acc_all+=htmlgen::div("header", "", ignoruj_podkreslniki(stanica->getNazwa()));
	acc_all+=htmlgen::div("header2", "", "Tory szlakowe połączone ze stacją");
	string acc_table, acc0;
	acc0+=htmlgen::div("ruch_stacja_pociag_time", "", "Id pociągu");
	acc0+=htmlgen::div("ruch_stacja_pociag_text", "", "Ze stacji");
	acc0+=htmlgen::div("ruch_stacja_pociag_text", "", "Poprzednia stacja");
	acc0+=htmlgen::div("ruch_stacja_pociag_time", "", "Tor wj.");
	acc0+=htmlgen::div("ruch_stacja_pociag_time", "", "Czas wj.");
	acc0+=htmlgen::div("ruch_stacja_pociag_time", "", "Czas wyj.");
	acc0+=htmlgen::div("ruch_stacja_pociag_time", "", "Tor wyj.");
	acc0+=htmlgen::div("ruch_stacja_pociag_text", "", "Następna stacja");
	acc0+=htmlgen::div("ruch_stacja_pociag_text", "", "Do stacji");
	acc_table+=htmlgen::div("head_line", "", acc0);
	for(int i=0; i<ruch->przejazdy.size(); i++)
	{
		acc0="";
		string acc3, acc4, acc5, acc6, acc7, acc8;
		string type="normal_line";
		Pociag* poc;
		if(ruch->przejazdy[i].first!=NULL)
		{
			acc3=totimesec(ruch->przejazdy[i].first->czasWyjazduZToru);
			acc5=stacjaLink(ruch->przejazdy[i].first->getStacjaPoczatkowa());
			acc7=torLinkShort(ruch->przejazdy[i].first->ruchPoTorze->tor);
			poc=ruch->przejazdy[i].first->pociag;
		}
		else
		{
			poc=ruch->przejazdy[i].second->pociag;
			if(bazac.poprzedniPociag(poc)!=NULL)
			{
				Pociag* poprz=bazac.poprzedniPociag(poc);
				acc5="<i>"+ignoruj_podkreslniki(poprz->getIdPociagu()+" "+totimemin(poprz->getCzasStop()))+"</i>";
			}
			type="start_line";
		}
		if(ruch->przejazdy[i].second!=NULL)
		{
			if(ruch->przejazdy[i].first!=NULL)
			{
				if(ruch->przejazdy[i].first->czasWyjazduZToru==ruch->przejazdy[i].second->czasWjazduNaTor)
				{
					type="nostop_line";
				}
				else
				{
					if(abs(ruch->przejazdy[i].first->czasWyjazduZToru-ruch->przejazdy[i].second->czasWjazduNaTor)!=poc->czasPostojuPlanowego[stanica])
					{
						type="waiting_line";
					}
				}
			}
			acc4=totimesec(ruch->przejazdy[i].second->czasWjazduNaTor);
			acc6=stacjaLink(ruch->przejazdy[i].second->getStacjaKoncowa());
			acc8=torLinkShort(ruch->przejazdy[i].second->ruchPoTorze->tor);
		}
		else
		{
			type="stop_line";
			if(bazac.nastepnyPociag(poc)!=NULL)
			{
				Pociag* poprz=bazac.nastepnyPociag(poc);
				acc6="<i>"+ignoruj_podkreslniki(poprz->getIdPociagu()+" "+totimemin(poprz->getCzasStart()))+"</i>";
			}
		}
		string acc9=htmlgen::link("pociag"+poc->getIdPociagu()+".html", ignoruj_podkreslniki(poc->getIdPociagu()));
		string acc1=stacjaLink(poc->getStacjaPoczatkowa());
		if(poc->getStacjaPoczatkowa()==stanica)
			acc1="";
		string acc2=stacjaLink(poc->getStacjaKoncowa());
		if(poc->getStacjaKoncowa()==stanica)
			acc2="";
		acc0+=htmlgen::div("ruch_stacja_pociag_time", "", acc9);
		acc0+=htmlgen::div("ruch_stacja_pociag_text", "", acc1);
		acc0+=htmlgen::div("ruch_stacja_pociag_text", "", acc5);
		acc0+=htmlgen::div("ruch_stacja_pociag_time", "", acc7);
		acc0+=htmlgen::div("ruch_stacja_pociag_time", "", acc3);
		acc0+=htmlgen::div("ruch_stacja_pociag_time", "", acc4);
		acc0+=htmlgen::div("ruch_stacja_pociag_time", "", acc8);
		acc0+=htmlgen::div("ruch_stacja_pociag_text", "", acc6);
		acc0+=htmlgen::div("ruch_stacja_pociag_text", "", acc2);
		acc_table+=htmlgen::div(type, "", acc0);
	}
	acc_table=htmlgen::div("ruch_stacja_pociag", "", acc_table);
	set <TorSzlakowy*> toryWych=stanica->getToryWychodzace();
	set <TorSzlakowy*>::iterator it1=toryWych.begin();
	while(it1!=toryWych.end())
	{
		acc_all+=torLinkLong(*it1)+"</br>";
		it1++;
	}
	acc_all+=htmlgen::div("header2", "", "Tabela ruchu przez stację - całość");
	acc_all+=acc_table;
	acc_all+=htmlgen::div("header2", "", "Zajętość torów przez pociągi");
	acc_all+=tabelaZajetosciTorowStacyjnych(liczbaPoc, bazac.wszystkieTypy());
	htmlgen::zlozHtml(acc_all, str, "style.css");
}

string warnings(BazaObiegow& bazac, BazaInfryKolejowej* bazaa, BazaRuchuKolejowego* bazab)
{
	string wynik;
	vector <string> typ=bazac.wszystkieTypy();
	for(int i=0; i<bazaa->wszystkieStacje().size(); i++)
	{
		LiczbaPociagowStacja liczbaPoc(bazac, bazaa->wszystkieStacje()[i], bazaa, bazab);
		for(int j=0; j<typ.size(); j++)
		{
			if(liczbaPoc.obiegiPoczatkowe(typ[j])!=liczbaPoc.obiegiKoncowe(typ[j]))
			{
				wynik+="<b>"+ignoruj_podkreslniki(bazaa->wszystkieStacje()[i]->getNazwa());
				wynik+=+": Obiegi typu "+typ[j]+" niezrównoważone; wjazdowe: ";
				wynik+=inttostring(liczbaPoc.obiegiPoczatkowe(typ[j]))+" wyjazdowe: "+inttostring(liczbaPoc.obiegiKoncowe(typ[j]))+"</b></br>";
			}
		}
	}
	return wynik;
}
string zestawienieTabelaStacje(BazaObiegow& bazac, BazaInfryKolejowej* bazaa, BazaRuchuKolejowego* bazab)
{
	string wynik;
	vector <string> typ=bazac.wszystkieTypy();
	for(int i=0; i<bazaa->wszystkieStacje().size(); i++)
	{
		LiczbaPociagowStacja liczbaPoc(bazac, bazaa->wszystkieStacje()[i], bazaa, bazab);
		
		wynik+=inttostring(i)+" "+stacjaLink(bazaa->wszystkieStacje()[i])+" ";
		wynik+=inttostring(liczbaPoc.obiegiPoczatkowe())+" ";
		wynik+=inttostring(liczbaPoc.maksymalnaZajetoscOdstawczych())+" ";
		wynik+=inttostring(liczbaPoc.maksymalnaZajetoscWszystkich())+" ";
		wynik+="</br>\n";
	}
	return wynik;
}
void generujzestawienie (BazaObiegow& bazac, BazaInfryKolejowej* bazaa, BazaRuchuKolejowego* bazab)
{
	string str="data/zestawienie.html";
	stringstream plik;
	plik<<warnings(bazac, bazaa, bazab);
	plik<<"<table>";
	int s1=bazac.liczbaObiegow();
	vector <string> obiegi_linijki;
	plik<<htmlgen::div("header2", "", "Lista stacji");
	plik<<zestawienieTabelaStacje(bazac, bazaa, bazab)<<endl;
	plik<<htmlgen::div("header2", "", "Lista przystanków");
	for(int i=0; i<bazaa->wszystkiePrzystanki().size(); i++)
		plik<<i<<" "<<przystanekLink(bazaa->wszystkiePrzystanki()[i])<<"</br>"<<endl;
	plik<<htmlgen::div("header2", "", "Lista torów");
	for(int i=0; i<bazaa->wszystkieTory().size(); i++)
	{
		plik<<torLinkLong(bazaa->wszystkieTory()[i])<<" "<<bazaa->wszystkieTory()[i]->additive<<" >> "<<endl;
		vector <pair <int, PrzystanekKolejowy*> > przyst=bazaa->wszystkieTory()[i]->getPrzystanki();
		for(int j=0; j<przyst.size(); j++)
		{
			plik<<przyst[j].second->getId()<<" "<<przystanekLink(przyst[j].second)<<" >> "<<endl;
		}
		plik<<"</br>"<<endl;
	}
	plik<<htmlgen::div("header2", "", "Lista obiegów");
	for(int i=0; i<s1; i++)
	{
		int obieg_distance=0;
		int obieg_czas_calk;
		int obieg_czas_start;
		int obieg_czas_stop;
		int obieg_czas_jazd=0;
		int obieg_stracony=0;
		StacjaKolejowa* obieg_stacja_koncowa;
		StacjaKolejowa* obieg_stacja_poczatkowa;
		plik<<"<tr><td colspan=\"8\"><b>OBIEG "<<i<<"</b></br></td></tr>"<<endl;
		vector <Pociag*> data=bazac.getObieg(i);
		int s2=data.size();
		for(int j=0; j<s2; j++)
		{
			Pociag* akt=data[j];
			if(j==0)
			{
				obieg_stacja_poczatkowa=akt->getStacjaPoczatkowa();
				obieg_czas_calk=akt->getCzasStart();
				obieg_czas_start=akt->getCzasStart();
			}
			if(j==s2-1)
			{
				obieg_stacja_koncowa=akt->getStacjaKoncowa();
				obieg_czas_calk=akt->getCzasStop()-obieg_czas_calk;
				obieg_czas_stop=akt->getCzasStop();
			}
			obieg_czas_jazd+=akt->czasPrzejazdu();
			obieg_distance+=akt->getDistance();
			//TODO obieg_stracony+=akt.stracony_czas;
			plik<<"<tr>";
			string a1="<td><a href=\"pociag"+akt->getIdPociagu()+".html\">"+akt->getIdPociagu()+"</a></td><td>"+inttostring(/*STRACONYCZAS TODO*/ 0)+"<td>";
			string a2="<td>"+inttostring(akt->czasPrzejazdu()/60)+"</td>";
			string a21="<td>"+inttostring(akt->getDistance())+"</td>";
			string a33="<a href=\"stacja"+inttostring(akt->getStacjaPoczatkowa()->getId())+".html\">"+ignoruj_podkreslniki(akt->getStacjaPoczatkowa()->getNazwa())+"</a>";
			string a34=totimemin(akt->getCzasStart());
			string a44=totimemin(akt->getCzasStop());
			string a43="<a href=\"stacja"+inttostring(akt->getStacjaKoncowa()->getId())+".html\">"+ignoruj_podkreslniki(akt->getStacjaKoncowa()->getNazwa())+"</a>";
			string a3="<td>"+a33+"</td><td>"+a34+"</td>";
			string a4="<td>"+a43+"</td><td>"+a44+"</td>";
			plik<<a1<<a2<<a21<<a3<<a4<<endl;
			plik<<"</tr>";
		}
		string a2="<td>"+inttostring(obieg_czas_calk/60)+"</td>";
		string a21="<td>"+inttostring(obieg_czas_jazd/60)+"</td>";
		string a22="<td>"+inttostring(obieg_stracony/60)+"</td>";
		string a23="<td>"+inttostring(obieg_distance)+"</td>";
		string a24="<td><a href=\"stacja"+inttostring(obieg_stacja_poczatkowa->getId())+".html\">"+ignoruj_podkreslniki(obieg_stacja_poczatkowa->getNazwa())+"</a></td>";
		string a25="<td><a href=\"stacja"+inttostring(obieg_stacja_koncowa->getId())+".html\">"+ignoruj_podkreslniki(obieg_stacja_koncowa->getNazwa())+"</a></td>";
		string a26="<td>"+totimemin(obieg_czas_start)+"</td>";
		string a27="<td>"+totimemin(obieg_czas_stop)+"</td>";
		obiegi_linijki.push_back("<tr>"+a2+a21+a22+a23+a24+a26+a25+a27+"</tr>");
	}
	plik<<"</table>";
	plik<<htmlgen::div("header2", "", "Zestawienie obiegów")<<"<table>"<<endl;
	for(int i=0; i<s1; i++)
	{
		plik<<obiegi_linijki[i]<<endl;
	}
	plik<<"</table>";
	htmlgen::zlozHtml(plik.str(), str, "style.css");
}

vector <pair <int, bool> > tabelaRuchuPoc2(PrzejazdPociaguPrzezTorSzlakowy* rpoc)
{
	vector <pair<int, bool > > wynik;
	if(rpoc->czyWlasciwyKierunekJazdy)
	{
		wynik.push_back(pair<int, bool>(rpoc->czasWjazduNaTor, rpoc->pociag->czyStopNaStacji(rpoc->getStacjaPoczatkowa())));
		for(int i=0; i<rpoc->postojePociagu.size(); i++)
		{
			int w1=rpoc->postojePociagu[i].odjazd;
			int w2=rpoc->postojePociagu[i].przyjazd;
			if(w1==w2)
			{
				wynik.push_back(pair<int, bool>(w1, false));
			}
			else
				wynik.push_back(pair<int, bool>(w1, true));
		}
		wynik.push_back(pair<int, bool>(rpoc->czasWyjazduZToru, rpoc->pociag->czyStopNaStacji(rpoc->getStacjaKoncowa())));
	}
	else
	{
		wynik.push_back(pair<int, bool>(rpoc->czasWyjazduZToru, rpoc->pociag->czyStopNaStacji(rpoc->getStacjaKoncowa())));
		for(int i=rpoc->postojePociagu.size()-1; i>=0; i--)
		{
			int w1=rpoc->postojePociagu[i].odjazd;
			int w2=rpoc->postojePociagu[i].przyjazd;
			if(w1==w2)
			{
				wynik.push_back(pair<int, bool>(w1, false));
			}
			else
				wynik.push_back(pair<int, bool>(w1, true));
		}

		wynik.push_back(pair<int, bool>(rpoc->czasWjazduNaTor, rpoc->pociag->czyStopNaStacji(rpoc->getStacjaPoczatkowa())));
	}
	return wynik;
}
string tabelaRuchuPoc (vector <pair<int, bool> > tr1)
{
	string wynik;
	for(int i=0; i<tr1.size(); i++)
	{
		string wtf;
		if(!tr1[i].second)
		{
			wtf=htmlgen::div("tortable_cell_time_nonstop", "", totimesec(tr1[i].first));
		}
		else
		{
			wtf=htmlgen::div("tortable_cell_time", "", totimesec(tr1[i].first));
		}
		wynik+=wtf;
	}
	return wynik;
}

bool srt2 (PrzejazdPociaguPrzezTorSzlakowy* alfa, PrzejazdPociaguPrzezTorSzlakowy* beta)
{
	if(alfa->czasWjazduNaTor<beta->czasWjazduNaTor)
		return true;
	return false;
}

string tabelaRuchu(RuchPociagowPoTorzeSzlakowym* ruch)
{
	vector <PrzejazdPociaguPrzezTorSzlakowy*> przejazdy;
	for(int i=0; i<ruch->ruchKierunekNormalny.size(); i++)
		przejazdy.push_back(ruch->ruchKierunekNormalny[i]);
	for(int i=0; i<ruch->ruchKierunekPrzeciwny.size(); i++)
		przejazdy.push_back(ruch->ruchKierunekPrzeciwny[i]);
	sort(przejazdy.begin(), przejazdy.end(), srt2);
	vector <vector <pair <int, bool> > > wynik2;
	for(int i=0; i<przejazdy.size(); i++)
	{
		wynik2.push_back(tabelaRuchuPoc2(przejazdy[i]));
	}
	string wynik;
	string gora;
	string gora1;
	gora+=(htmlgen::div("tortable_cell_time", "", ""));
	gora+=(htmlgen::div("tortable_cell_text", "", ""));
	gora+=(htmlgen::div("tortable_cell_znaczek", "", ""));
	gora1+=(htmlgen::div("tortable_cell_time", "", ""));
	gora1+=(htmlgen::div("tortable_cell_text", "", ""));
	gora1+=(htmlgen::div("tortable_cell_znaczek", "", ""));
	gora+=(htmlgen::div("tortable_cell_time", "", stacjaLink(ruch->tor->getStacjaPoczatkowa())));
	gora1+=htmlgen::div("tortable_cell_time", "", "0");
	for(int i=0; i<ruch->tor->getPrzystanki().size(); i++)
	{
		gora+=htmlgen::div("tortable_cell_time", "", przystanekLink(ruch->tor->getPrzystanki()[i].second));
		gora1+=htmlgen::div("tortable_cell_time", "", inttostring((ruch->tor->getPrzystanki()[i].first)));
	}
	gora+=htmlgen::div("tortable_cell_time", "", stacjaLink(ruch->tor->getStacjaKoncowa()));
	gora1+=htmlgen::div("tortable_cell_time", "", inttostring((ruch->tor->getDlugoscWMetrach())));
	gora+=(htmlgen::div("tortable_cell_znaczek", "", ""));
	gora+=(htmlgen::div("tortable_cell_text", "", ""));
	gora1+=(htmlgen::div("tortable_cell_znaczek", "", ""));
	gora1+=(htmlgen::div("tortable_cell_text", "", ""));
	wynik+=htmlgen::div("tortable_line_h2", "", gora);
	wynik+=htmlgen::div("tortable_line_h1", "", gora1);
	for(int i=0; i<wynik2.size(); i++)
	{
		string foo=tabelaRuchuPoc(wynik2[i]);
		string znaczek = "&#8594;";
		if(!przejazdy[i]->czyWlasciwyKierunekJazdy)
		{
			znaczek = "&#8592;";
			foo=htmlgen::div("tortable_cell_znaczek", "", znaczek)+foo;
			foo=foo+htmlgen::div("tortable_cell_znaczek", "", znaczek);
			foo+=htmlgen::div("tortable_cell_text", "", stacjaLink(przejazdy[i]->pociag->getStacjaPoczatkowa()));
			foo=htmlgen::div("tortable_cell_text", "", stacjaLink(przejazdy[i]->pociag->getStacjaKoncowa()))+foo;
		}
		else
		{
			foo=htmlgen::div("tortable_cell_znaczek", "", znaczek)+foo;
			foo=foo+htmlgen::div("tortable_cell_znaczek", "", znaczek);
			foo+=htmlgen::div("tortable_cell_text", "", stacjaLink(przejazdy[i]->pociag->getStacjaKoncowa()));
			foo=htmlgen::div("tortable_cell_text", "", stacjaLink(przejazdy[i]->pociag->getStacjaPoczatkowa()))+foo;
		}
		foo=htmlgen::div("tortable_cell_time", "", htmlgen::link("pociag"+przejazdy[i]->pociag->getIdPociagu()+".html", przejazdy[i]->pociag->getIdPociagu()))+foo;
		wynik+=htmlgen::div("tortable_line", "", foo);
	}
	wynik+=htmlgen::div("tortable_line_h2", "", gora);
	return htmlgen::div("tortable", "", wynik);
}
void generujtorhtml (BazaInfryKolejowej* bazaa, BazaRuchuKolejowego* bazab, int nkol)
{
	string str="data/linia"+inttostring(nkol)+".html";
	stringstream plik;
	vector <TorSzlakowy*> torySzlakowe = bazaa->wszystkieTory();
	int stacja_pocz=torySzlakowe[nkol]->getStacjaPoczatkowa()->getId();
	int stacja_konc=torySzlakowe[nkol]->getStacjaKoncowa()->getId();
	RuchPociagowPoTorzeSzlakowym* ruch = bazab->torToRuch[torySzlakowe[nkol]];
	TorSzlakowy* tor = ruch->tor;
	plik<<htmlgen::div("header", "", "Tor "+inttostring(tor->getId())+" "+ignoruj_podkreslniki(tor->getStacjaPoczatkowa()->getNazwa())+" - "+ignoruj_podkreslniki(tor->getStacjaKoncowa()->getNazwa()+" "+tor->additive))<<endl;

	plik<<tabelaRuchu(ruch);
	htmlgen::zlozHtml(plik.str(), str, "style.css");
}
#endif
