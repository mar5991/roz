#ifndef BAZAINFRY
#define BAZAINFRY
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <cmath>
#include <vector>
using namespace std;
struct SBL
{
	int nastepstwoPociagowWMinutachKierunekNormalny;
	int nastepstwoPociagowWMinutachKierunekPrzeciwny;
	int normalny()
	{
		return nastepstwoPociagowWMinutachKierunekNormalny;
	}
	int przeciwny()
	{
		return nastepstwoPociagowWMinutachKierunekPrzeciwny;
	}
};
class StacjaKolejowa;
class PrzystanekKolejowy;
struct TorSzlakowyBuilder
{
	int id;
	int dlugoscWMetrach;
	vector <pair <int, PrzystanekKolejowy*> > przystanki; // odleglosc / id
	StacjaKolejowa* stacjaPoczatkowa;
	StacjaKolejowa* stacjaKoncowa;
	double predkoscMaksymalnaWKPH;
	SBL sbl;
};
class TorSzlakowy
{
	int id;
	int dlugoscWMetrach;
	vector <pair <int, PrzystanekKolejowy*> > przystanki; // odleglosc / id
	StacjaKolejowa* stacjaPoczatkowa;
	StacjaKolejowa* stacjaKoncowa;
	double predkoscMaksymalnaWKPH;
	SBL sbl;
	public:
	TorSzlakowy(TorSzlakowyBuilder builder);
	int getDlugoscWMetrach();
	StacjaKolejowa* getStacjaPoczatkowa();
	StacjaKolejowa* getStacjaKoncowa();
	double getPredkoscMaksymalnaWKPH();
	SBL getSBL();
	vector <pair <int, PrzystanekKolejowy*> > getPrzystanki()
	{
		return przystanki;
	}
};


struct PrzystanekKolejowyBuilder
{
	string nazwa;
	int id;
};

class PrzystanekKolejowy
{
	string nazwa;
	set <TorSzlakowy*> toryPrzechodzace;
	int id;
	public:
	int getId()
	{
		return id;
	}
	string getNazwa();
	PrzystanekKolejowy(PrzystanekKolejowyBuilder builder);
	void dodajTor(TorSzlakowy* torSzlakowy);
};

struct StacjaKolejowaBuilder
{
	string nazwa;
	int id;
};

class StacjaKolejowa
{
	set <TorSzlakowy*> toryWychodzace;
	string nazwa;
	int id;
	public:
	string getNazwa();
	StacjaKolejowa(StacjaKolejowaBuilder builder);
	void dodajTor(TorSzlakowy* torSzlakowy);
	int getId()
	{
		return id;
	}
};

struct BazaInfryKolejowejBuilder
{
	vector <StacjaKolejowa*> stacje;
	vector <TorSzlakowy*> tory;
	vector <PrzystanekKolejowy*> przystanki;
};

class BazaInfryKolejowej
{
	public:
	vector <StacjaKolejowa*> stacje;
	vector <TorSzlakowy*> tory;
	vector <PrzystanekKolejowy*> przystanki;
	public:
	BazaInfryKolejowej(BazaInfryKolejowejBuilder builder)
	{
		stacje=builder.stacje;
		tory=builder.tory;
		przystanki=builder.przystanki;
	}
	vector <StacjaKolejowa*> wszystkieStacje()
	{
		return stacje;
	}
	vector <TorSzlakowy*> wszystkieTory()
	{
		return tory;
	}
	vector <PrzystanekKolejowy*> wszystkiePrzystanki()
	{
		return przystanki;
	}
};

class BazaInfryKolejowejFromFile : public BazaInfryKolejowejBuilder
{
	public:
	void dodaj_stacje(stringstream& alfa)
	{
		StacjaKolejowaBuilder builder;
		alfa>>builder.nazwa;
		builder.id=stacje.size();
		stacje.push_back(new StacjaKolejowa(builder));
	}
	void dodaj_przystanek(stringstream& alfa)
	{
		PrzystanekKolejowyBuilder builder;
		alfa>>builder.nazwa;
		builder.id=przystanki.size();
		przystanki.push_back(new PrzystanekKolejowy(builder));
	}
	void dodaj_tor(stringstream& alfa)
	{
		TorSzlakowyBuilder builder;
		alfa>>builder.dlugoscWMetrach;
		int stacjaPoczatkowa;
		int stacjaKoncowa;
		alfa>>stacjaPoczatkowa;
		alfa>>stacjaKoncowa;
		builder.id=tory.size();
		builder.stacjaPoczatkowa=stacje[stacjaPoczatkowa];
		builder.stacjaKoncowa=stacje[stacjaKoncowa];
		alfa>>builder.predkoscMaksymalnaWKPH;
		alfa>>builder.sbl.nastepstwoPociagowWMinutachKierunekNormalny;
		alfa>>builder.sbl.nastepstwoPociagowWMinutachKierunekPrzeciwny;
		int a1, a2;
		while(alfa>>a1)
		{
			alfa>>a2;
			builder.przystanki.push_back(pair<int, PrzystanekKolejowy*> (a1, przystanki[a2]));
		}
		tory.push_back(new TorSzlakowy(builder));
	}
	public:
	BazaInfryKolejowejFromFile(string sciezka)
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
#endif
