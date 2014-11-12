#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "pkpbaza.hpp"
using namespace std;

struct plot_odc
{
	int xStart;
	int yStart;
	int xStop;
	int yStop;
	int style;
};

struct Plot
{
	int rozdzielczoscX;
	int rozdzielczoscY;
	vector <pair <double, string> > xTics;
	vector <pair <double, string> > yTics;
	double xMin, xMax, yMin, yMax;
	vector <plot_odc> odcinki;
	Plot(int rozX, int rozY, double xMinW, double xMaxW, double yMinW, double yMaxW)
	{
		rozdzielczoscX=rozX;
		rozdzielczoscY=rozY;
		xMin=xMinW;
		xMax=xMaxW;
		yMin=yMinW;
		yMax=yMaxW;
	}
	void dodajOdcinek(plot_odc odc)
	{
		odcinki.push_back(odc);
	}
	void dodajXTic(double wart, string name)
	{
		xTics.push_back(pair<double, string>(wart, name));
	}
	void dodajYTic(double wart, string name)
	{
		yTics.push_back(pair<double, string>(wart, name));
	}
	void drawXTic(fstream& alfa)
	{
		alfa<<"set xtics (";
		for(int i=0; i<xTics.size(); i++)
		{
			alfa<<"\""<<xTics[i].second<<"\" "<<xTics[i].first<<", ";
		}
		alfa<<")"<<endl;
	}
	void drawYTic(fstream& alfa)
	{
		alfa<<"set ytics (";
		for(int i=0; i<yTics.size(); i++)
		{
			alfa<<"\""<<yTics[i].second<<"\" "<<yTics[i].first<<", ";
		}
		alfa<<")"<<endl;
	}
	void draw(string out)
	{
		fstream pom1("pom.txt", ios::out | ios::trunc);
		fstream pom2("pom2.txt", ios::out | ios::trunc);
		for(int i=0; i<odcinki.size(); i++)
		{
			pom2<<odcinki[i].xStart<<"	"<<odcinki[i].yStart<<"	";
		}
		pom2<<endl;
		for(int i=0; i<odcinki.size(); i++)
		{
			pom2<<odcinki[i].xStop<<"	"<<odcinki[i].yStop<<"	";
		}
		pom2.close();
		drawXTic(pom1);
		drawYTic(pom1);
		pom1<<"set term png size "<<rozdzielczoscX<<", "<<rozdzielczoscY<<endl;
		pom1<<"set grid\n set key off"<<endl;
		pom1<<"set output \""<<out<<"\""<<endl;
		pom1<<"plot ["<<xMin<<":"<<xMax<<"]["<<yMin<<":"<<yMax<<"] ";
		for(int i=0; i<odcinki.size(); i++)
		{
			pom1<<"\"pom2.txt\" using "<<i*2+1<<":"<<i*2+2<<"wi li linetype ";
			pom1<<odcinki[i].style<<" lw 3";
			if(i<odcinki.size()-1)
			{
				pom1<<", ";
			}
		}
		pom1.close();
		system("gnuplot pom.txt");
		system("rm pom.txt");
		system("rm pom2.txt");
	}
};
void easyTorPlot(Plot& plo, PrzejazdPociaguPrzezTorSzlakowy* przej, int start, int stop, bool wlasciwy)
{
	plot_odc plc;
	plc.xStart=przej->czasWjazduNaTor;
	plc.xStop=przej->czasWyjazduZToru;
	plc.style=0;
	plc.yStart=start;
	plc.yStop=stop;
	if(wlasciwy==przej->czyWlasciwyKierunekJazdy)
	{
		plc.xStart=przej->czasWjazduNaTor;
		plc.xStop=przej->czasWyjazduZToru;
	}
	else
	{
		plc.xStop=przej->czasWjazduNaTor;
		plc.xStart=przej->czasWyjazduZToru;
	}
	plo.dodajOdcinek(plc);
}
void easyTorPlot(Plot& plo, PrzejazdPociaguPrzezTorSzlakowy* przej, int start, int stop)
{
	RuchPociagowPoTorzeSzlakowym* ruch = przej->ruchPoTorze;
	for(int i=0; i<ruch->ruchKierunekNormalny.size(); i++)
	{
		PrzejazdPociaguPrzezTorSzlakowy* akt=ruch->ruchKierunekNormalny[i];
		easyTorPlot(plo, akt, start, stop, przej->czyWlasciwyKierunekJazdy);
	}
	for(int i=0; i<ruch->ruchKierunekPrzeciwny.size(); i++)
	{
		PrzejazdPociaguPrzezTorSzlakowy* akt=ruch->ruchKierunekPrzeciwny[i];
		easyTorPlot(plo, akt, start, stop, przej->czyWlasciwyKierunekJazdy);
	}
}
void easyTrainPlot(Pociag* poc)
{
	int distance=0;
	Plot plo(1600, 3200, poc->getCzasStart()-30*60,  poc->getCzasStop()+30*60, 0, poc->getDistance());
	for(int i=0; i<poc->przejazdyPrzezTorSzlakowy.size(); i++)
	{
		easyTorPlot(plo, poc->przejazdyPrzezTorSzlakowy[i], distance, distance+poc->przejazdyPrzezTorSzlakowy[i]->ruchPoTorze->tor->getDlugoscWMetrach());
		plot_odc plc;
		plc.yStart=distance;
		plo.dodajYTic(distance, poc->przejazdyPrzezTorSzlakowy[i]->getStacjaPoczatkowa()->getNazwa());
		plc.xStart=poc->przejazdyPrzezTorSzlakowy[i]->czasWjazduNaTor;
		plc.xStop=poc->przejazdyPrzezTorSzlakowy[i]->czasWyjazduZToru;
		distance+=poc->przejazdyPrzezTorSzlakowy[i]->ruchPoTorze->tor->getDlugoscWMetrach();
		plc.yStop=distance;
		plc.style=1;
		plo.dodajOdcinek(plc);
	}
	plo.draw("data/"+poc->getIdPociagu()+".png");
}