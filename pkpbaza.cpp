#include "pkpbaza.hpp"
vector <PostojPociagu> Pociag::getPostojePociagu()
	{
		vector <PostojPociagu> wynik(przejazdyPrzezTorSzlakowy.size()+1);
		for(int i=0; i<przejazdyPrzezTorSzlakowy.size(); i++)
		{
			wynik[i].timeOdjazd=przejazdyPrzezTorSzlakowy[i]->czasWjazduNaTor;
			wynik[i+1].timePrzyjazd=przejazdyPrzezTorSzlakowy[i]->czasWyjazduZToru;
			wynik[i].typ=false;
			wynik[i+1].typ=false;
			wynik[i].stacja=przejazdyPrzezTorSzlakowy[i]->ruchPoTorze->tor->getStacjaPoczatkowa();
			wynik[i+1].stacja=przejazdyPrzezTorSzlakowy[i]->ruchPoTorze->tor->getStacjaKoncowa();
		}
		return wynik;
	}
int czasPrzejazduWSekundach(double vstart_mps, double vstop_mps, double vmax, double odl, double przyspieszenie)
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
