#include "bazainfry.hpp"
TorSzlakowy::TorSzlakowy(TorSzlakowyBuilder builder)
	{
		id=builder.id;
		dlugoscWMetrach=builder.dlugoscWMetrach;
		przystanki=builder.przystanki;
		stacjaPoczatkowa=builder.stacjaPoczatkowa;
		stacjaKoncowa=builder.stacjaKoncowa;
		predkoscMaksymalnaWKPH=builder.predkoscMaksymalnaWKPH;
		sbl=builder.sbl;
		stacjaPoczatkowa->dodajTor(this);
		stacjaKoncowa->dodajTor(this);
		additive=builder.additive;
		for(int i=0; i<przystanki.size(); i++)
		{
			(przystanki[i].second)->dodajTor(this);
		}
	}
	int TorSzlakowy::getDlugoscWMetrach()
	{
		return dlugoscWMetrach;
	}
	StacjaKolejowa* TorSzlakowy::getStacjaPoczatkowa()
	{
		return stacjaPoczatkowa;
	}
	StacjaKolejowa* TorSzlakowy::getStacjaKoncowa()
	{
		return stacjaKoncowa;
	}
	double TorSzlakowy::getPredkoscMaksymalnaWKPH()
	{
		return predkoscMaksymalnaWKPH;
	}
	SBL TorSzlakowy::getSBL()
	{
		return sbl;
	}
	string PrzystanekKolejowy::getNazwa()
	{
		return nazwa;
	}
	PrzystanekKolejowy::PrzystanekKolejowy(PrzystanekKolejowyBuilder builder)
	{
		id = builder.id;
		nazwa = builder.nazwa;
	}
	void PrzystanekKolejowy::dodajTor(TorSzlakowy* torSzlakowy)
	{
		toryPrzechodzace.insert(torSzlakowy);
	}
	string StacjaKolejowa::getNazwa()
	{
		return nazwa;
	}
	StacjaKolejowa::StacjaKolejowa(StacjaKolejowaBuilder builder)
	{
		id=builder.id;
		nazwa=builder.nazwa;
	}
	void StacjaKolejowa::dodajTor(TorSzlakowy* torSzlakowy)
	{
		toryWychodzace.insert(torSzlakowy);
	}
