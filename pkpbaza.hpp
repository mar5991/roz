#ifndef PKPBAZAHPP
#define PKPBAZAHPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>
#include <vector>
#include "bazainfry.hpp"
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

int czasPrzejazduWSekundach(double vstart_mps, double vstop_mps, double vmax, double odl, double przyspieszenie);
struct Pociag;
struct RuchPociagowPoTorzeSzlakowym;
struct PostojPociaguNaPrzystanku
{
	int przyjazd;
	int odjazd;
	PrzystanekKolejowy* przystanek;
};
struct PrzejazdPociaguPrzezTorSzlakowy
{
	Pociag* pociag;
	RuchPociagowPoTorzeSzlakowym* ruchPoTorze;
	vector <PostojPociaguNaPrzystanku> postojePociagu;
	int czasWjazduNaTor;
	int czasWyjazduZToru;
	bool czyWlasciwyKierunekJazdy;
	StacjaKolejowa* getStacjaPoczatkowa();
	StacjaKolejowa* getStacjaKoncowa();
};
struct PostojPociagu
{
	bool typ; //Stacja false / Przystanek true
	StacjaKolejowa* stacja;
	PrzystanekKolejowy* przystanek;
	int timePrzyjazd;
	int timeOdjazd;
};
struct Pociag
{
	private:
	string idPociagu;
	public:
	Pociag(string id)
	{
		idPociagu=id;
	}
	vector <PrzejazdPociaguPrzezTorSzlakowy*> przejazdyPrzezTorSzlakowy;
	string getIdPociagu()
	{
		return idPociagu;
	}
	string getTypPociagu()
	{
		string wynik;
		bool dodaj=false;
		for(int i=0; i<idPociagu.length(); i++)
		{
			if(dodaj)
			{
				wynik+=idPociagu[i];
			}
			if(idPociagu[i]=='_')
				dodaj=true;
		}
		return wynik;
	}
	void dodajTorZPrzodu(PrzejazdPociaguPrzezTorSzlakowy* nowy)
	{
		przejazdyPrzezTorSzlakowy.push_back(nowy);
	}
	void dodajTorZTylu(PrzejazdPociaguPrzezTorSzlakowy* nowy)
	{
		przejazdyPrzezTorSzlakowy.insert(przejazdyPrzezTorSzlakowy.begin(), nowy);
	}
	StacjaKolejowa* getStacjaPoczatkowa();
	StacjaKolejowa* getStacjaKoncowa();
	int getCzasStart()
	{
		return przejazdyPrzezTorSzlakowy[0]->czasWjazduNaTor;
	}
	int getDistance();
	int getCzasStop()
	{
		return przejazdyPrzezTorSzlakowy[liczbaOdwiedzonychTorow()-1]->czasWyjazduZToru;
	}
	int liczbaStacji()
	{
		return przejazdyPrzezTorSzlakowy.size()+1;
	}
	int liczbaOdwiedzonychTorow()
	{
		return przejazdyPrzezTorSzlakowy.size();
	}
	StacjaKolejowa* getStacja(int id);
	void dodajTor(PrzejazdPociaguPrzezTorSzlakowy* nowy, bool wal)
	{
		if(wal)
			dodajTorZPrzodu(nowy);
		else
			dodajTorZTylu(nowy);
	}
	int czasPrzejazdu()
	{
		return getCzasStop()-getCzasStart();
	}
	bool czyStopNaStacji(StacjaKolejowa* stacja)
	{
		if(getStacjaPoczatkowa()==stacja)
			return true;
		if(getStacjaKoncowa()==stacja)
			return true;
		for(int i=0; i<liczbaOdwiedzonychTorow()-1; i++)
		{
			if(przejazdyPrzezTorSzlakowy[i]->getStacjaKoncowa()==stacja)
			{
				if(przejazdyPrzezTorSzlakowy[i]->czasWyjazduZToru==przejazdyPrzezTorSzlakowy[i+1]->czasWjazduNaTor)
				{
					return false;
				}
				return true;
			}
		}
		return false;
	}
	vector <PostojPociagu> getPostojePociagu();
};
struct RuchPociagowPoTorzeSzlakowym
{
	TorSzlakowy* tor;
	vector <PrzejazdPociaguPrzezTorSzlakowy*> ruchKierunekNormalny;
	vector <PrzejazdPociaguPrzezTorSzlakowy*> ruchKierunekPrzeciwny;
	~RuchPociagowPoTorzeSzlakowym()
	{
		for(auto wsk : ruchKierunekNormalny)
			delete wsk;
		for(auto wsk : ruchKierunekPrzeciwny)
			delete wsk;
	}
	bool czyDaSiePuscicPociagBrakSBL(int czasWjazdu, int czasPrzejazdu, vector <PrzejazdPociaguPrzezTorSzlakowy*> &jegoKierunek)
	{
		int czasWyjazdu=czasWjazdu+czasPrzejazdu;
		int s1=jegoKierunek.size();
		for(int i=0; i<s1; i++)
		{
			if(jegoKierunek[i]->czasWjazduNaTor>=czasWjazdu)
				if(jegoKierunek[i]->czasWjazduNaTor<=czasWyjazdu)
					return false;
			if(jegoKierunek[i]->czasWyjazduZToru>=czasWjazdu)
				if(jegoKierunek[i]->czasWyjazduZToru<=czasWyjazdu)
					return false;
		}
		return true;
	}
	bool czyDaSiePuscicPociagJegoKierunek(int czasWjazdu, int czasPrzejazdu, vector <PrzejazdPociaguPrzezTorSzlakowy*> &jegoKierunek, int sblDlaKierunku)
	{
		int czasWyjazdu=czasWjazdu+czasPrzejazdu;
		if(sblDlaKierunku==-1)
			return czyDaSiePuscicPociagBrakSBL(czasWjazdu, czasPrzejazdu, jegoKierunek);
		int s1=jegoKierunek.size();
		for(int i=0; i<s1; i++)
		{
			if(jegoKierunek[i]->czasWjazduNaTor>(czasWjazdu-sblDlaKierunku))
				if(jegoKierunek[i]->czasWyjazduZToru<(czasWyjazdu+sblDlaKierunku))
					return false;
			if(jegoKierunek[i]->czasWjazduNaTor<(czasWjazdu+sblDlaKierunku))
				if(jegoKierunek[i]->czasWyjazduZToru>(czasWyjazdu-sblDlaKierunku))
					return false;
		}
		return true;
	}
	bool czyDaSiePuscicPociag(int czasWjazdu, int czasPrzejazdu, bool czyWlasciwyKierunekJazdy)
	{
		if(czyWlasciwyKierunekJazdy)
		{
			bool alpha=czyDaSiePuscicPociagJegoKierunek(czasWjazdu, czasPrzejazdu, ruchKierunekNormalny, tor->getSBL().normalny());
			bool beta=czyDaSiePuscicPociagBrakSBL(czasWjazdu, czasPrzejazdu, ruchKierunekPrzeciwny);
			return alpha && beta;
		}
		else
		{
			bool alpha=czyDaSiePuscicPociagJegoKierunek(czasWjazdu, czasPrzejazdu, ruchKierunekPrzeciwny, tor->getSBL().przeciwny());
			bool beta=czyDaSiePuscicPociagBrakSBL(czasWjazdu, czasPrzejazdu, ruchKierunekNormalny);
			return alpha && beta;
		}
	}
	RuchPociagowPoTorzeSzlakowym(TorSzlakowy* torW)
	{
		tor=torW;
	}
	int zliczPrzystankiNaLinii(set <PrzystanekKolejowy*> przystanki)
	{
		vector <pair <int, PrzystanekKolejowy*> > przystankiNa = tor->getPrzystanki();
		int licznik=0;
		for(int i=0; i<przystankiNa.size(); i++)
		{
			if(przystanki.find(przystankiNa[i].second)!=przystanki.end())
				licznik++;
		}
		return licznik;
	}
	vector <PostojPociaguNaPrzystanku> generujPostoje(int czasWjazdu, int czasPrzejazdu, set <PrzystanekKolejowy*> przystanki, bool czyWlasciwyKierunekJazdy, int czasNaPrzystanek)
	{
		vector <PostojPociaguNaPrzystanku> wynik;
		int liczba=zliczPrzystankiNaLinii(przystanki);
		int surowyCzasPrzejazdu = czasPrzejazdu-czasNaPrzystanek*liczba;
		int dlugoscToru = tor -> getDlugoscWMetrach();
		if(czyWlasciwyKierunekJazdy)
		{
			int przerobionePostoje=0;
			vector <pair <int, PrzystanekKolejowy*> > przystankiNa = tor->getPrzystanki();
			for(int i=0; i<przystankiNa.size(); i++)
			{
				PostojPociaguNaPrzystanku foo;
				foo.przystanek=przystankiNa[i].second;
				if(przystanki.find(przystankiNa[i].second)!=przystanki.end())
				{
					foo.przyjazd=czasWjazdu+surowyCzasPrzejazdu*przystankiNa[i].first/dlugoscToru+przerobionePostoje*czasNaPrzystanek;
					przerobionePostoje++;
					foo.odjazd=czasWjazdu+surowyCzasPrzejazdu*przystankiNa[i].first/dlugoscToru+przerobionePostoje*czasNaPrzystanek;
				}
				else
				{
					foo.przyjazd=czasWjazdu+surowyCzasPrzejazdu*przystankiNa[i].first/dlugoscToru+przerobionePostoje*czasNaPrzystanek;
					foo.odjazd=czasWjazdu+surowyCzasPrzejazdu*przystankiNa[i].first/dlugoscToru+przerobionePostoje*czasNaPrzystanek;
				}
				wynik.push_back(foo);
			}
		}
		else
		{
			int przerobionePostoje=0;
			vector <pair <int, PrzystanekKolejowy*> > przystankiNa = tor->getPrzystanki();
			for(int i=przystankiNa.size()-1; i>=0; i--)
			{
				PostojPociaguNaPrzystanku foo;
				foo.przystanek=przystankiNa[i].second;
				if(przystanki.find(przystankiNa[i].second)!=przystanki.end())
				{
					foo.przyjazd=czasWjazdu+surowyCzasPrzejazdu*(dlugoscToru-przystankiNa[i].first)/dlugoscToru+przerobionePostoje*czasNaPrzystanek;
					przerobionePostoje++;
					foo.odjazd=czasWjazdu+surowyCzasPrzejazdu*(dlugoscToru-przystankiNa[i].first)/dlugoscToru+przerobionePostoje*czasNaPrzystanek;
				}
				else
				{
					foo.przyjazd=czasWjazdu+surowyCzasPrzejazdu*(dlugoscToru-przystankiNa[i].first)/dlugoscToru+przerobionePostoje*czasNaPrzystanek;
					foo.odjazd=czasWjazdu+surowyCzasPrzejazdu*(dlugoscToru-przystankiNa[i].first)/dlugoscToru+przerobionePostoje*czasNaPrzystanek;
				}
				wynik.push_back(foo);
			}
		}
		return wynik;
	}
	bool dodajPociag(int czasWjazdu, int czasPrzejazdu, bool czyPrzeciwny, Pociag* pociag, bool zPrzodu, set <PrzystanekKolejowy*> przystanki, int czasNaPrzystanek)
	{
		if(czyDaSiePuscicPociag(czasWjazdu, czasPrzejazdu, !czyPrzeciwny))
		{
			PrzejazdPociaguPrzezTorSzlakowy* nowy = new PrzejazdPociaguPrzezTorSzlakowy();
			nowy -> ruchPoTorze = this;
			nowy -> czasWjazduNaTor = czasWjazdu;
			nowy -> czasWyjazduZToru = czasWjazdu+czasPrzejazdu;
			if(!czyPrzeciwny)
			{
				nowy -> czyWlasciwyKierunekJazdy = true;
				ruchKierunekNormalny.push_back(nowy);		
			}
			else
			{
				nowy -> czyWlasciwyKierunekJazdy = false;
				ruchKierunekPrzeciwny.push_back(nowy);		
			}
			nowy -> pociag = pociag;
			nowy -> postojePociagu = generujPostoje(czasWjazdu, czasPrzejazdu, przystanki,!czyPrzeciwny, czasNaPrzystanek);
			pociag->dodajTor(nowy, zPrzodu);
			return true;
		}
		return false;
	}
};
bool srt3(pair <PrzejazdPociaguPrzezTorSzlakowy*, PrzejazdPociaguPrzezTorSzlakowy*>& alfa, pair <PrzejazdPociaguPrzezTorSzlakowy*, PrzejazdPociaguPrzezTorSzlakowy*>& beta);
struct RuchPociagowPrzezStacje
{
	StacjaKolejowa* stacja;
	vector <pair <PrzejazdPociaguPrzezTorSzlakowy*, PrzejazdPociaguPrzezTorSzlakowy*> > przejazdy;
	RuchPociagowPrzezStacje (StacjaKolejowa* kand)
	{
		stacja = kand;
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
};
struct BazaRuchuKolejowego
{
	vector <RuchPociagowPoTorzeSzlakowym*> tory;
	vector <Pociag*> pociagi;
	map <TorSzlakowy*, RuchPociagowPoTorzeSzlakowym*> torToRuch;
	map <StacjaKolejowa*, RuchPociagowPrzezStacje*> stacjaToRuch;
	BazaInfryKolejowej* bazaInfry;
	
	BazaRuchuKolejowego(vector <kand_pociag> kand, BazaInfryKolejowej* infra) : bazaInfry(infra)
	{
		vector <TorSzlakowy*> torySzlak=bazaInfry->wszystkieTory();
		for(int i=0; i<torySzlak.size(); i++)
		{
			tory.push_back(new RuchPociagowPoTorzeSzlakowym(torySzlak[i]));
			torToRuch[torySzlak[i]]=tory[i];
		}
		for(int i=0; i<kand.size(); i++)
		{
			dodaj_pociag(kand[i]);
		}
		uzupelnijStacjaToRuch();
	}
	vector <Pociag*> getPociagi()
	{
		return pociagi;
	}
	Pociag* getPociag(string id)
	{
		for(int i=0; i<pociagi.size(); i++)
		{
			if(pociagi[i]->getIdPociagu()==id)
				return pociagi[i];
		}
		return NULL;

	}
	RuchPociagowPoTorzeSzlakowym* getRuchPoTorze(TorSzlakowy* tor)
	{
		return torToRuch[tor];
	}
	~BazaRuchuKolejowego()
	{
		for(int i=0; i<tory.size(); i++)
			delete tory[i];
		for(int i=0; i<pociagi.size(); i++)
			delete pociagi[i];
		for(auto& it1 : stacjaToRuch)
			delete it1.second;
	}
	private:
	void uzupelnijPociagiStacje(Pociag* p)
	{
		for(int i=0; i<(p->przejazdyPrzezTorSzlakowy.size()-1); i++)
		{
			StacjaKolejowa* teraz=p->przejazdyPrzezTorSzlakowy[i]->getStacjaKoncowa();
			stacjaToRuch[teraz]->przejazdy.push_back(pair<PrzejazdPociaguPrzezTorSzlakowy*, PrzejazdPociaguPrzezTorSzlakowy*>(p->przejazdyPrzezTorSzlakowy[i], p->przejazdyPrzezTorSzlakowy[i+1]));
		}
		StacjaKolejowa* start = p->getStacjaPoczatkowa();
		stacjaToRuch[start]->przejazdy.push_back(pair<PrzejazdPociaguPrzezTorSzlakowy*, PrzejazdPociaguPrzezTorSzlakowy*>(NULL, p->przejazdyPrzezTorSzlakowy[0]));
		StacjaKolejowa* stop = p->getStacjaKoncowa();
		stacjaToRuch[stop]->przejazdy.push_back(pair<PrzejazdPociaguPrzezTorSzlakowy*, PrzejazdPociaguPrzezTorSzlakowy*>(p->przejazdyPrzezTorSzlakowy[p->przejazdyPrzezTorSzlakowy.size()-1], NULL));
	
	}
	void uzupelnijStacjaToRuch()
	{
		vector <StacjaKolejowa*> stacje=bazaInfry->wszystkieStacje();
		for(int i=0; i<stacje.size(); i++)
		{
			RuchPociagowPrzezStacje* nowy = new RuchPociagowPrzezStacje(stacje[i]);
			stacjaToRuch[stacje[i]]=nowy;
		}
		for(int i=0; i<pociagi.size(); i++)
		{
			uzupelnijPociagiStacje(pociagi[i]);
		}
		for(int i=0; i<stacje.size(); i++)
		{
			sort(stacjaToRuch[stacje[i]]->przejazdy.begin(), stacjaToRuch[stacje[i]]->przejazdy.end(), srt3);
		}
	}
	bool czy_pociag(int time1, vector <int> id_torow, vector <int> czasy_przejazdu, vector <bool> czy_przec)
	{
		int s1=id_torow.size();
		for(int i=0; i<s1; i++)
		{
			if(torToRuch[(bazaInfry->wszystkieTory())[i]]->czyDaSiePuscicPociag(time1, czasy_przejazdu[i], !czy_przec[i]))
				return false;
			time1+=czasy_przejazdu[i];
		}
		return true;
	}
	set <PrzystanekKolejowy*> transformPrzystanki (set <int> przystanki)
	{
		set <PrzystanekKolejowy*> wynik;
		set<int>::iterator it1=przystanki.begin();
		while(it1!=przystanki.end())
		{
			wynik.insert(bazaInfry->wszystkiePrzystanki()[*it1]);
			it1++;
		}
		return wynik;
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
			if((tory[kand.tory_przejazdowe[i]]->tor->getStacjaPoczatkowa()->getId()==kand.stacje_postojowe[i+1].first))
				czyprzec=1;
			bool czystop1;
			bool czystop2;
			int alfa=kand.tory_przejazdowe[i];
			int l1=tory[alfa]->tor->getDlugoscWMetrach();
			vector <int> posrednie;
			vector <pair <int, PrzystanekKolejowy*> > przyT=tory[alfa]->tor->getPrzystanki();
			int s2=przyT.size();
			if(!czyprzec)
				for(int j=0; j<s2; j++)
				{
					int k1=przyT[j].first;
					int k2=(przyT[j].second)->getId();
					if(kand.przystanki_postojowe.find(k2)!=kand.przystanki_postojowe.end())
					{
						posrednie.push_back(k1);
					}
				}
			else
				for(int j=s2-1; j>=0; j--)
				{
					int k1=przyT[j].first;
					int k2=(przyT[j].second)->getId();
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
						ap1=double(tory[kand.tory_przejazdowe[i]]->tor->getPredkoscMaksymalnaWKPH())/3.6;
					}
				}

				timeruchu+=czasPrzejazduWSekundach(ap1, 0, double(tory[kand.tory_przejazdowe[i]]->tor->getPredkoscMaksymalnaWKPH())/3.6, posrednie[j]-ostposredni, kand.przyspieszenie);
				timeruchu+=kand.czas_postoju_przystanek;
				ostposredni=posrednie[j];
			}
			double ap1=0;
			if(s3==0)
			{

				if(kand.stacje_postojowe[i].second==0)
				{
					ap1=double(tory[kand.tory_przejazdowe[i]]->tor->getPredkoscMaksymalnaWKPH())/3.6;
				}
			}

			double ap2=0;
			if(kand.stacje_postojowe[i+1].second==0)
			{
				ap2=double(tory[kand.tory_przejazdowe[i]]->tor->getPredkoscMaksymalnaWKPH())/3.6;
			}
			int add=czasPrzejazduWSekundach(ap1, ap2, double(tory[kand.tory_przejazdowe[i]]->tor->getPredkoscMaksymalnaWKPH())/3.6, l1-ostposredni, kand.przyspieszenie);
			timeruchu+=add;

			czasyprzejazdu.push_back(timeruchu);
		}
		return czasyprzejazdu;
	}
	private:
	void dodaj_pociag(kand_pociag kand)
	{
		Pociag* nowy = new Pociag(kand.kod);
		vector <int> czasyprzejazdu=wyznacz_czasy_przejazdu(kand);
		int s1=kand.tory_przejazdowe.size();
		int akttime=kand.time_start;
		int nkola=0;
		while(kand.stacje_postojowe[nkola].first!=kand.stacja_srodkowa)
			nkola++;
		int graniczny=nkola;
		int akttime2=akttime-kand.stacje_postojowe[graniczny].second;
		for(int i=graniczny-1; i>=0; i--)
		{
			bool czyprzec=0;
			if(tory[kand.tory_przejazdowe[i]]->tor->getStacjaPoczatkowa()->getId()==kand.stacje_postojowe[i+1].first)
				czyprzec=1;
			while(!tory[kand.tory_przejazdowe[i]]->czyDaSiePuscicPociag(akttime2-czasyprzejazdu[i], czasyprzejazdu[i], !czyprzec))
			{
				akttime2-=10;
			}
			tory[kand.tory_przejazdowe[i]]->dodajPociag(akttime2-czasyprzejazdu[i], czasyprzejazdu[i], czyprzec, nowy, false, transformPrzystanki(kand.przystanki_postojowe), 20);
			akttime2-=czasyprzejazdu[i]+kand.stacje_postojowe[i].second;
		}

		for(int i=graniczny; i<s1; i++)
		{
			bool czyprzec=0;
			if(tory[kand.tory_przejazdowe[i]]->tor->getStacjaPoczatkowa()->getId()==kand.stacje_postojowe[i+1].first)
				czyprzec=1;
			while(!tory[kand.tory_przejazdowe[i]]->czyDaSiePuscicPociag(akttime, czasyprzejazdu[i], !czyprzec))
			{
				akttime+=10;
			}
			tory[kand.tory_przejazdowe[i]]->dodajPociag(akttime, czasyprzejazdu[i], czyprzec, nowy, true, transformPrzystanki(kand.przystanki_postojowe), 20);
			akttime+=czasyprzejazdu[i]+kand.stacje_postojowe[i+1].second;
		}
		pociagi.push_back(nowy);
	}

	/*vector <punkt_posr> dodaj_przystanki(int timestart, int czas_przejazdu, bool czyprzec, kand_pociag& kand, int i)
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
	}*/

};



string totimesec(int time);
string totimemin(int time);
#endif
