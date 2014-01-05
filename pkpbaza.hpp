#ifndef PKPBAZAHPP
#define PKPBAZAHPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <cmath>
#include <vector>
using namespace std;

/*string kodgen(int id)
{
	stringstream tmp;
	tmp<<id;
	tmp<<"_KM";
	string wynik;
	tmp>>wynik;
	return wynik;
}*/

int czasprzejazdu(double vstart_mps, double vstop_mps, double vmax, double odl, double przyspieszenie)
{
	double t1=(vmax-vstart_mps)/przyspieszenie;
	double s1=vstart_mps*t1+przyspieszenie*t1*t1/2;
	double t2=(vmax-vstop_mps)/przyspieszenie;
	double s2=vstop_mps*t1+przyspieszenie*t1*t1/2;
	if(odl-s1-s2>0)
		return (odl-s1-s2)/vmax+t1+t2;
	double tk1=(vstart_mps/przyspieszenie);
	double tk2=(vstop_mps/przyspieszenie);
	double sk1=przyspieszenie*tk1*tk1/2;
	double sk2=przyspieszenie*tk2*tk2/2;
	double tpart=2*sqrt((sk1+sk2+odl)/przyspieszenie);
	return tpart-tk1-tk2;
}

struct tor
{
	int dlugosc_m;
	vector <pair <int, int> > przystanki; // odleglosc / id
	int stacja_poczatkowa;
	int stacja_koncowa;
	double vmax;
	int nastepstwo_normal;
	int nastepstwo_przec;
};

struct pociag_na_torze
{
	string id;
	int time_start;
	int time_stop;
};

struct tor_data
{
	tor* alpha;
	vector <pociag_na_torze> nor;
	vector <pociag_na_torze> prz;
	bool czy_pociag(int timestart, int timeprzej, vector <pociag_na_torze> &normal, vector <pociag_na_torze> &przeciwny)
	{
		int timestop=timestart+timeprzej;
		int s1=normal.size();
		for(int i=0; i<s1; i++)
		{
			if(alpha->nastepstwo_normal!=-1)
			{
				if(!((timestop>normal[i].time_stop && timestart>normal[i].time_start) || (timestart<normal[i].time_start && timestop<normal[i].time_stop)))
					return false;
				if(timestart>normal[i].time_start-alpha->nastepstwo_normal && timestart<normal[i].time_start+alpha->nastepstwo_normal)
					return false;
				if(timestop>normal[i].time_stop-alpha->nastepstwo_normal && timestop<normal[i].time_stop+alpha->nastepstwo_normal)
					return false;
			}
			else
			{
				if(!((timestop>normal[i].time_stop && timestart>normal[i].time_start) || (timestart<normal[i].time_start && timestop<normal[i].time_stop)))
					return false;
				if(timestart>=normal[i].time_start && timestart<=normal[i].time_stop)
					return false;
				if(timestop>=normal[i].time_start && timestop<=normal[i].time_stop)
					return false;
			}
		}
		s1=przeciwny.size();
		for(int i=0; i<s1; i++)
		{
			if(!((timestop>przeciwny[i].time_start && timestart>przeciwny[i].time_stop) || (timestart<przeciwny[i].time_stop && timestop<przeciwny[i].time_start)))
				return false;
		}
		return true;
	}
	bool czy_pociag(int timestart, int timeprzej, bool czyprzec)
	{
		if(!czyprzec)
			return czy_pociag(timestart, timeprzej, nor, prz);
		return czy_pociag(timestart, timeprzej, prz, nor);
	}
	tor_data(tor* od)
	{
		alpha=od;
	}
	bool dodaj_pociag(string id, int timestart, int timeprzej, bool czyprzec)
	{
		if(!czyprzec)
		{
			if(!czy_pociag(timestart, timeprzej, nor, prz))
				return false;
			pociag_na_torze nowy;
			nowy.id=id;
			nowy.time_start=timestart;
			nowy.time_stop=timestart+timeprzej;
			nor.push_back(nowy);
		}
		else
		{
			if(!czy_pociag(timestart, timeprzej, prz, nor))
				return false;
			pociag_na_torze nowy;
			nowy.id=id;
			nowy.time_start=timestart;
			nowy.time_stop=timestart+timeprzej;
			prz.push_back(nowy);
		}
		return true;
	}
	
};
struct kand_pociag
{
	private:
	void wypelnijtrase(string sciezka)
	{
		fstream plik;
		plik.open(sciezka.c_str());
		char tmp1[10000];
		plik.getline(tmp1,10000);
		stringstream p1;
		p1<<tmp1;
		int p2;
		while(p1>>p2)
		{
			przystanki_postojowe.insert(p2);
		}
		plik.getline(tmp1,10000);
		stringstream p3;
		p3<<tmp1;
		int p4;
		while(p3>>p4)
		{
			tory_przejazdowe.push_back(p4);
		}
		while(plik.getline(tmp1, 10000))
		{
			int alpha, gamma;
			stringstream beta;
			beta<<tmp1;
			beta>>alpha;
			beta>>gamma;
			stacje_postojowe.push_back(pair <int, int>(alpha, gamma));
		}
		plik.close();
	}
	public:
	int stacja_srodkowa;
	string kod;
	int time_start; //w sekundach od północy
	int vmaxkph; //w kilometrach na godzinę
	double przyspieszenie; //w metrach na sekundę kwadrat
	int czas_postoju_przystanek; //w sekundach
	vector <int> tory_przejazdowe;
	set <int> przystanki_postojowe;
	vector <pair <int, int> > stacje_postojowe; //id, czas postoju w sekundach, postój 0 oznacza jego brak; liczba rekordów w wektorze: n+1, gdzie n=liczba rekordów w tory_przejazdowe
	void wypelnijtrase(vector <string> sciezka)
	{
		tory_przejazdowe.clear();
		przystanki_postojowe.clear();
		stacje_postojowe.clear();
		int s1=sciezka.size();
		for(int i=0; i<s1; i++)
		{
			wypelnijtrase(sciezka[i]);
			if(i<s1-1)
			{
				stacje_postojowe.pop_back();
			}
		}
	}
};

struct baza_sieci_wewn
{
	vector <string> nazwy_stacji;
	vector <tor*> tory;
	vector <string> przystanki;
	vector <set <int> > tory_przystanek;
	vector <set <int> > tory_stacja;
	void dodaj_stacje(stringstream& alfa)
	{
		string nazwa;
		alfa>>nazwa;
		set <int> pusty;
		nazwy_stacji.push_back(nazwa);
		tory_stacja.push_back(pusty);
	}
	vector <pair <int, int> > lista_przystanki(int id_toru)
	{
		return (tory[id_toru])->przystanki;
	}
	void dodaj_tor(stringstream& alfa)
	{
		tor* nowy=new tor;
		int k1;
		alfa>>nowy->dlugosc_m;
		alfa>>nowy->stacja_poczatkowa;
		alfa>>nowy->stacja_koncowa;
		alfa>>nowy->vmax;
		alfa>>nowy->nastepstwo_normal;
		alfa>>nowy->nastepstwo_przec;
		int a1, a2;
		while(alfa>>a1)
		{
			alfa>>a2;
			nowy->przystanki.push_back(pair<int, int> (a1,a2));
			tory_przystanek[a2].insert(tory.size());
		}

		tory_stacja[nowy->stacja_poczatkowa].insert(tory.size());
		tory_stacja[nowy->stacja_koncowa].insert(tory.size());
		tory.push_back(nowy);
	}
	void dodaj_przystanek(stringstream& alfa)
	{
		string nazwa;
		alfa>>nazwa;
		set <int> pusty;
		przystanki.push_back(nazwa);
		tory_przystanek.push_back(pusty);
	}
	void dodaj_siec(string sciezka)
	{
		fstream plik;
		plik.open(sciezka.c_str());
		char data[10000];
		int tryb=0;
		while(plik.getline(data, 10000))
		{
			stringstream temp;
			temp<<data;
			string alfa1;
			temp>>alfa1;
			if(alfa1=="S")
			{
				dodaj_stacje(temp);
			}
			if(alfa1=="T")
			{

				dodaj_tor(temp);
			}
			if(alfa1=="P")
			{

				dodaj_przystanek(temp);
			}
		}
		plik.close();
	}
};

struct punkt_posr
{
	bool typ; // 0 - stacja, 1 - przystanek
	int czas_przyjazdu;
	int czas_odjazdu;
	int id_infr; //id stacji lub przystanku
};

struct pociag
{
	int distance;
	int stracony_czas;
	string kod;
	vector <punkt_posr> punkty;
	int czas_przejazdu()
	{
		int s1=punkty.size();
		return punkty[s1-1].czas_przyjazdu-punkty[0].czas_odjazdu;
	}
	int stacja_poczatkowa()
	{
		return punkty[0].id_infr;
	}
	int stacja_koncowa()
	{
		return punkty[punkty.size()-1].id_infr;
	}
	int czas_start()
	{
		return punkty[0].czas_odjazdu;
	}
	int czas_stop()
	{
		return punkty[punkty.size()-1].czas_przyjazdu;
	}
};

struct baza_pociagow_wewn
{
	baza_sieci_wewn* bs;
	vector <tor_data> tory;
	vector <set <string> > tory_pociagi;
	vector <set <string> > stacje_pociagi;
	vector <pociag> pociagi;
	baza_pociagow_wewn(baza_sieci_wewn* x) : stacje_pociagi(x->nazwy_stacji.size())
	{
		bs=x;
		int s1=bs->tory.size();
		for(int i=0; i<s1; i++)
		{
			set <string> pusty;
			tory_pociagi.push_back(pusty);
			tor_data nowy(bs->tory[i]);
			tory.push_back(nowy);
		}
	}
	string nazwa_postojowa(int pociag_id, int posr_id) //ZWRACA NAZWĘ STACJI LUB PRZYSTANKU POCIĄGU pociag_id, NA KOLEJNYM PRZYSTANKU posr_id
	{
		bool typ=pociagi[pociag_id].punkty[posr_id].typ;
		if(typ==0)
		{
			return bs->nazwy_stacji[pociagi[pociag_id].punkty[posr_id].id_infr];
		}
		return bs->przystanki[pociagi[pociag_id].punkty[posr_id].id_infr];
	}
	bool czy_pociag(int time1, vector <int> id_torow, vector <int> czasy_przejazdu, vector <bool> czy_przec)
	{
		int s1=id_torow.size();
		for(int i=0; i<s1; i++)
		{
			if(!tory[id_torow[i]].czy_pociag(time1, czasy_przejazdu[i], czy_przec[i]))
				return false;
			time1+=czasy_przejazdu[i];
		}
		return true;
	}
	int pociag_mozliwy_przod(int timestart, vector <int> id_torow, vector <int> czasy_przejazdu, vector <bool> czy_przec)
	{
			while(!czy_pociag(timestart, id_torow, czasy_przejazdu, czy_przec))
				timestart+=20;
		return timestart;
	}
	int pociag_mozliwy_tyl(int timestop, vector <int> id_torow, vector <int> czasy_przejazdu, vector <bool> czy_przec)
	{
		int sumprzej=0;
		int s1=czasy_przejazdu.size();
		for(int i=0; i<s1; i++)
		{
			sumprzej+=czasy_przejazdu[i];
		}
		int timestart=timestop-sumprzej;
		while(!czy_pociag(timestart, id_torow, czasy_przejazdu, czy_przec))
				timestart-=20;
		return timestart;
	}
	vector <int> wyznacz_czasy_przejazdu(kand_pociag& kand)
	{
		vector <int> czasyprzejazdu;
		int s1=kand.tory_przejazdowe.size();
		for(int i=0; i<s1; i++)
		{
			bool czyprzec=0;
			if((tory[kand.tory_przejazdowe[i]].alpha->stacja_poczatkowa==kand.stacje_postojowe[i+1].first))
				czyprzec=1;
			bool czystop1;
			bool czystop2;
			int alfa=kand.tory_przejazdowe[i];
			int s2=bs->tory[alfa]->przystanki.size();
			int l1=bs->tory[alfa]->dlugosc_m;
			vector <int> posrednie;
			if(!czyprzec)
				for(int j=0; j<s2; j++)
				{
					int k1=bs->tory[alfa]->przystanki[j].first;
					int k2=bs->tory[alfa]->przystanki[j].second;
					if(kand.przystanki_postojowe.find(k2)!=kand.przystanki_postojowe.end())
					{
						posrednie.push_back(k1);
					}
				}
			else
				for(int j=s2-1; j>=0; j--)
				{
					int k1=bs->tory[alfa]->przystanki[j].first;
					int k2=bs->tory[alfa]->przystanki[j].second;
					if(kand.przystanki_postojowe.find(k2)!=kand.przystanki_postojowe.end())
					{
						posrednie.push_back(l1-k1);
					}
				}
			int s3=posrednie.size();
			int ostposredni=0;
			int timeruchu=0;
			for(int j=0; j<s3; j++)
			{
				double ap1=0;
				if(j==0)
				{
					if(kand.stacje_postojowe[i].second==0)
					{
						ap1=double(bs->tory[kand.tory_przejazdowe[i]]->vmax)/3.6;
					}
				}
				timeruchu+=czasprzejazdu(ap1, 0, double(bs->tory[kand.tory_przejazdowe[i]]->vmax)/3.6, posrednie[j]-ostposredni, kand.przyspieszenie);
				timeruchu+=kand.czas_postoju_przystanek;
				ostposredni=posrednie[j];
			}
			double ap1=0;
			if(s3==0)
			{

				if(kand.stacje_postojowe[i].second==0)
				{
					ap1=double(bs->tory[kand.tory_przejazdowe[i]]->vmax)/3.6;
				}
			}
			double ap2=0;
			if(kand.stacje_postojowe[i+1].second==0)
			{
				ap2=double(bs->tory[kand.tory_przejazdowe[i]]->vmax)/3.6;
			}
			int add=czasprzejazdu(ap1, ap2, double(bs->tory[kand.tory_przejazdowe[i]]->vmax)/3.6, l1-ostposredni, kand.przyspieszenie);
			timeruchu+=add;
			czasyprzejazdu.push_back(timeruchu);
		}
		return czasyprzejazdu;
	}
	vector <punkt_posr> dodaj_przystanki(int timestart, int czas_przejazdu, bool czyprzec, kand_pociag& kand, int i)
	{
		vector <punkt_posr> punkty;
			int alfa=kand.tory_przejazdowe[i];
			int dlug=bs->tory[alfa]->dlugosc_m;
			if(!czyprzec)
			{
				int s2=bs->tory[alfa]->przystanki.size();
				for(int j=0; j<s2; j++)
				{
					int k1=bs->tory[alfa]->przystanki[j].first;
					int k2=bs->tory[alfa]->przystanki[j].second;
					double prop=(double)k1/(double)dlug;
					double tsr=timestart+(czas_przejazdu*prop);
					if(kand.przystanki_postojowe.find(k2)!=kand.przystanki_postojowe.end())
					{
						punkt_posr p1;
						p1.id_infr=k2;
						p1.typ=1;
						int alpha9=kand.czas_postoju_przystanek/2;
						p1.czas_przyjazdu=tsr-alpha9;
						p1.czas_odjazdu=tsr+alpha9;
						punkty.push_back(p1);
					}
					else
					{
						punkt_posr p1;
						p1.id_infr=k2;
						p1.typ=1;
						p1.czas_przyjazdu=tsr;
						p1.czas_odjazdu=tsr;
						punkty.push_back(p1);
					}
				}
			}
			else
			{
				int s2=bs->tory[alfa]->przystanki.size();
				for(int j=s2-1; j>=0; j--)
				{
					int k1=bs->tory[alfa]->przystanki[j].first;
					int k2=bs->tory[alfa]->przystanki[j].second;
					double prop=1.0-(double)k1/(double)dlug;
					double tsr=timestart+(czas_przejazdu*prop);
					if(kand.przystanki_postojowe.find(k2)!=kand.przystanki_postojowe.end())
					{
						punkt_posr p1;
						p1.id_infr=k2;
						p1.typ=1;
						int alpha9=kand.czas_postoju_przystanek/2;
						p1.czas_przyjazdu=tsr-alpha9;
						p1.czas_odjazdu=tsr+alpha9;
						punkty.push_back(p1);
					}
					else
					{
						punkt_posr p1;
						p1.typ=1;
						p1.id_infr=k2;
						p1.czas_przyjazdu=tsr;
						p1.czas_odjazdu=tsr;
					punkty.push_back(p1);
					}
				}
			}
			return punkty;
	}
	vector <punkt_posr> generuj_puste_stacje(kand_pociag& kand)
	{
		vector <punkt_posr> empty;
		int s1=kand.tory_przejazdowe.size();
		for(int i=0; i<=s1; i++)
		{
			punkt_posr nowy;
			nowy.typ=0;
			nowy.id_infr=kand.stacje_postojowe[i].first;
			empty.push_back(nowy);
		}
		return empty;
	}
	void dodaj_pociag(kand_pociag kand)
	{
		pociag nowy;
		nowy.stracony_czas=0;
		nowy.distance=0;
		nowy.kod=kand.kod;
		vector <int> czasyprzejazdu=wyznacz_czasy_przejazdu(kand);
		int s1=kand.tory_przejazdowe.size();
		vector <punkt_posr> posrednie_stacje=generuj_puste_stacje(kand);
		vector <vector <punkt_posr> > posrednie_przystanki(s1);
		int akttime=kand.time_start;
		int nkola=0;
		while(kand.stacje_postojowe[nkola].first!=kand.stacja_srodkowa)
			nkola++;
		int graniczny=nkola;
		int akttime2=akttime-kand.stacje_postojowe[graniczny].second;
		for(int i=graniczny-1; i>=0; i--)
		{
			nowy.distance+=(tory[kand.tory_przejazdowe[i]].alpha)->dlugosc_m;
			tory_pociagi[kand.tory_przejazdowe[i]].insert(kand.kod);
			stacje_pociagi[kand.stacje_postojowe[i].first].insert(kand.kod);
			bool czyprzec=0;
			if(tory[kand.tory_przejazdowe[i]].alpha->stacja_poczatkowa==kand.stacje_postojowe[i+1].first)
				czyprzec=1;
			while(!tory[kand.tory_przejazdowe[i]].czy_pociag(akttime2-czasyprzejazdu[i], czasyprzejazdu[i], czyprzec))
			{
				nowy.stracony_czas+=10;
				akttime2-=10;
			}
			tory[kand.tory_przejazdowe[i]].dodaj_pociag(kand.kod, akttime2-czasyprzejazdu[i], czasyprzejazdu[i], czyprzec);
			posrednie_stacje[i+1].czas_przyjazdu=akttime2;
			posrednie_stacje[i].czas_odjazdu=akttime2-czasyprzejazdu[i];
			vector <punkt_posr> abc=dodaj_przystanki(akttime2-czasyprzejazdu[i], czasyprzejazdu[i], czyprzec, kand, i);
			posrednie_przystanki[i]=abc;
			akttime2-=czasyprzejazdu[i]+kand.stacje_postojowe[i].second;
		}
		for(int i=graniczny; i<s1; i++)
		{
			nowy.distance+=(tory[kand.tory_przejazdowe[i]].alpha)->dlugosc_m;
			tory_pociagi[kand.tory_przejazdowe[i]].insert(kand.kod);
			stacje_pociagi[kand.stacje_postojowe[i].first].insert(kand.kod);
			bool czyprzec=0;
			if(tory[kand.tory_przejazdowe[i]].alpha->stacja_poczatkowa==kand.stacje_postojowe[i+1].first)
				czyprzec=1;
			while(!tory[kand.tory_przejazdowe[i]].czy_pociag(akttime, czasyprzejazdu[i], czyprzec))
			{
				nowy.stracony_czas+=10;
				akttime+=10;
			}
			posrednie_stacje[i].czas_odjazdu=akttime;
			posrednie_stacje[i+1].czas_przyjazdu=akttime+czasyprzejazdu[i];
			tory[kand.tory_przejazdowe[i]].dodaj_pociag(kand.kod, akttime, czasyprzejazdu[i], czyprzec);
			vector <punkt_posr> abc=dodaj_przystanki(akttime, czasyprzejazdu[i], czyprzec, kand, i);
			posrednie_przystanki[i]=abc;
			akttime+=czasyprzejazdu[i]+kand.stacje_postojowe[i+1].second;
		}

		stacje_pociagi[kand.stacje_postojowe[s1].first].insert(kand.kod);
		posrednie_stacje[s1].czas_odjazdu=posrednie_stacje[s1].czas_przyjazdu+kand.stacje_postojowe[s1].second;
		posrednie_stacje[0].czas_przyjazdu=posrednie_stacje[0].czas_odjazdu-kand.stacje_postojowe[0].second;
		for(int i=0; i<s1; i++)
		{
			nowy.punkty.push_back(posrednie_stacje[i]);
			int s2=posrednie_przystanki[i].size();
			for(int j=0; j<s2; j++)
			{
				nowy.punkty.push_back(posrednie_przystanki[i][j]);
			}
		}
		nowy.punkty.push_back(posrednie_stacje[s1]);
		pociagi.push_back(nowy);
	}
};

string totimesec(int time)
{
	stringstream as;
	stringstream am;
	stringstream ag;
	string ss;
	string mm;
	string gg;
	int sec=time%60;
	int min=(time/60)%60;
	int godz=time/3600;
	as<<sec;
	am<<min;
	ag<<godz;
	as>>ss;
	am>>mm;
	ag>>gg;
	if(ss.length()==1)
		ss="0"+ss;
	if(mm.length()==1)
		mm="0"+mm;
	if(gg.length()==1)
		gg="0"+gg;
	string wynik=gg+":"+mm+":"+ss;
	return wynik;
}

string totimemin(int time)
{
	stringstream as;
	stringstream am;
	stringstream ag;
	string ss;
	string mm;
	string gg;
	int sec=time%60;
	int min=(time/60)%60;
	int godz=time/3600;
	as<<sec;
	am<<min;
	ag<<godz;
	as>>ss;
	am>>mm;
	ag>>gg;
	if(ss.length()==1)
		ss="0"+ss;
	if(mm.length()==1)
		mm="0"+mm;
	if(gg.length()==1)
		gg="0"+gg;
	string wynik=gg+":"+mm;
	return wynik;
}

class baza_pociagow
{
	baza_pociagow_wewn* alfa;
	public:
	baza_pociagow(baza_sieci_wewn* bz1)
	{
		alfa=new baza_pociagow_wewn(bz1);
	}
	string nazwa_postojowa(string kod, int posr_id) 
	{
		int s1=alfa->pociagi.size();
		for(int i=0; i<s1; i++)
		{
			if(alfa->pociagi[i].kod==kod)
				return alfa->nazwa_postojowa(i, posr_id);
		}
	}
	pociag poc(int id)
	{
		return alfa->pociagi[id];
	}
	int liczba_pociagow()
	{
		return alfa->pociagi.size();
	}
	pociag poc(string kod)
	{
		//TODO
		int s1=alfa->pociagi.size();
		for(int i=0; i<s1; i++)
		{
			if(alfa->pociagi[i].kod==kod)
				return alfa->pociagi[i];
		}
	}
	void dodaj_pociag(kand_pociag p)
	{
		alfa->dodaj_pociag(p);
	}
	~baza_pociagow()
	{
		delete alfa;
	}
	vector <pociag_na_torze> lista_toru_nor(int id)
	{
		return alfa->tory[id].nor;
	}
	vector <pociag_na_torze> lista_toru_prz(int id)
	{
		return alfa->tory[id].prz;
	}
	set <string> lista_pociagow_tor (int id)
	{
		return alfa->tory_pociagi[id];
	}
	set <string> lista_pociagow_stacja (int id)
	{
		return alfa->stacje_pociagi[id];
	}
	
};

class baza_sieci
{
	baza_sieci_wewn* bz1;
	public:
	set <int> powiazane_tory (int id, bool czy_stacja)
	{
		if(czy_stacja)
			return bz1->tory_stacja[id];
		return bz1->tory_przystanek[id];
	}
	string nazwa_stacji(int id)
	{
		return bz1->nazwy_stacji[id];
	}
	string nazwa_przystanku(int id)
	{
		return bz1->przystanki[id];
	}
	int numer_stacji_pocz(int id_toru)
	{
		return bz1->tory[id_toru]->stacja_poczatkowa;
	}
	int numer_stacji_konc(int id_toru)
	{
		return bz1->tory[id_toru]->stacja_koncowa;
	}
	int liczba_stacji()
	{
		return bz1->nazwy_stacji.size();
	}
	int liczba_torow()
	{
		return bz1->tory.size();
	}
	baza_sieci(string siec)
	{
		bz1=new baza_sieci_wewn;
		bz1->dodaj_siec(siec);
	}
	vector <pair <int, int> > przystanki(int id_toru)
	{
		return bz1->lista_przystanki(id_toru);
	}
	baza_pociagow generuj_baze()
	{
		baza_pociagow nowa(bz1);
		return nowa;
	}
	~baza_sieci()
	{
		delete bz1;
	}
};
#endif
