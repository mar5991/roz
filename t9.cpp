#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "genalgpoc.hpp"
#include "t6.hpp"
using namespace std;
int main(int argc, char** argv)
{
	string baza_sciezka;
	cin>>baza_sciezka;
	baza_sieci bazaa(baza_sciezka);
	string abc;
	vector <string> lista_plikow;
	while(cin>>abc)
	{
		lista_plikow.push_back(abc);
	}
	algopoc alg(8, 25, 5, 5, &bazaa, lista_plikow);
	alg.generujprostrze("kupka", 10);
}
