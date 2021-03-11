#ifndef SPD_HPP
#define SPD_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include "tester.hpp"
#include "kopiec.hpp"

using namespace std;

struct Element 
{
	int R;
	int P;
	int Q;
	int index;
};

class Output
{
	public:
		int dlugosc;
		vector<int> permutacja;
};

/*inline int min_r(const vector<Element>& zestaw)
{
	int min = 999999;
	int ind = -1;
	
	for(unsigned int i = 0; i < zestaw.size(); ++i)
	{
		if(min > zestaw[i].R)
		{
			min = zestaw[i].R;
			ind = i;
		}
	}
	
	return ind;
}

inline int max_q(const vector<Element>& zestaw)
{
	int max = -1;
	int ind = -1;
	
	for(unsigned int i = 0; i < zestaw.size(); ++i)
	{
		if(max < zestaw[i].Q)
		{
			max = zestaw[i].Q;
			ind = i;
		}
	}
	
	return ind;
}*/

bool min_r(const Element& l, const Element& r) // Asceding order
{
	return l.R > r.R;
}

bool max_q(const Element& l, const Element& r) // Desceding order
{
	return l.Q < r.Q;
}

Output Schrage(const vector<Element>& wejscie)
{
	// Inicjalizacja wyjscia
	Output wyjscie;
	wyjscie.dlugosc = 0;
	wyjscie.permutacja.resize(wejscie.size());
		
	// Referencje - dla zachowania zgodnosci kodu z pseudokodem
	int& Cmax = wyjscie.dlugosc; // Minimalny czas wykonywania
	vector<int>& Pi = wyjscie.permutacja;
	// Zmienne lokalne
	int t = 0; // Chwila czasowa
	int k = 0; // Pozycja w permutacji
	// Kopce
	Heap<Element> kopiec_r(min_r, wejscie);
	Heap<Element> kopiec_q(max_q);
	// Dobra, jednak zmodyfikowalem, poniewaz uzywam elmentow zamiast indeksow
	// Generalnie: e to element, nie index, a zbiory G, N zawieraja elementy zamiast indeksow
	// N przerobilem na kopiec_r, G na kopiec_q
	while(kopiec_r.size() || kopiec_q.size())
	{
		label:
		while(kopiec_r.size() && kopiec_r.front().R <= t)
		{
			Element e = kopiec_r.front();
			kopiec_r.pop();
			kopiec_q.push(e);
		}
		if(!kopiec_q.size())
		{
			//t = N[min_r(N)].R;
			t = kopiec_r.front().R;
			goto label;
		}
		
		Element e = kopiec_q.front();
		kopiec_q.pop();		
		
		Pi[k] = e.index;
		t = t + e.P;
		Cmax = max(Cmax,t+e.Q);
		++k;
	}
						
	return wyjscie;
}

class SPDTester : public Tester<vector<Element>,Output>
{
	protected:
		vector<Element> WczytajPojedynczeWejscie(istream& wejscie) override
		{
			unsigned int liczba_elementow = 0;
			wejscie >> liczba_elementow;
			vector<Element> wyjscie(liczba_elementow);
			for(unsigned int i = 0; i < wyjscie.size(); ++i)
			{
				wejscie >> wyjscie[i].R;
				wejscie >> wyjscie[i].P;
				wejscie >> wyjscie[i].Q;
				wyjscie[i].index = i;
			}
			return wyjscie;
		}
	public:
		Output Algorytm(vector<Element>& wejscie, ostream& logs, const int& index)
		{
			//logs << wejscie.size() << endl;	for(unsigned int i = 0; i < wejscie.size(); ++i)	{ logs << wejscie[i].R << ' ' << wejscie[i].P << ' ' << wejscie[i].Q << endl; }
			return Schrage((const vector<Element>&)wejscie);
		}
		
		void WyswietlWynik(ostream& wyjscie,const Output& wynik, const int& index, const double& ms)
		{
			wyjscie << "Zakonczono przetwarzanie zestawu: " << index << endl;
			wyjscie << "Dlugosc uszeregowania: " << wynik.dlugosc << endl;
			wyjscie << "Czas pracy algorytmu: " << ms << " [ms]" << endl;
			wyjscie << "Permutacja:";
			for(unsigned int i = 0; i < wynik.permutacja.size(); ++i)
			{
				wyjscie << ' ' << (wynik.permutacja[i]+1);
			}
		}
		
		SPDTester(std::istream& we, std::ostream& wy, std::ostream& lo) : Tester(we, wy, lo) {}
};

#endif