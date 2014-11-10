#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "genalgpoc.hpp"
#include "t6.hpp"
using namespace std;
int main(int argc, char** argv)
{
	srand(time(NULL));
	string baza_sciezka;
	cin>>baza_sciezka;
	
	BazaInfryKolejowejFromFile bui(baza_sciezka);
	BazaInfryKolejowej* bazaa = new BazaInfryKolejowej(bui);
	string abc;
	vector <string> lista_plikow;
	while(cin>>abc)
	{
		lista_plikow.push_back(abc);
	}
	algopoc alg(8, 25, 5, 5, bazaa, lista_plikow);
	vector <int> wynik=alg.get_wynik(50);
	int s9=wynik.size();
	fstream plik;
	plik.open(argv[1]);
	plik<<baza_sciezka<<endl;
	for(int i=0; i<s9; i++)
	{
		plik<<wynik[i]<<endl;
	}
	for(int i=0; i<lista_plikow.size(); i++)
	{
		plik<<lista_plikow[i]<<endl;
	}
	plik.close();
}
