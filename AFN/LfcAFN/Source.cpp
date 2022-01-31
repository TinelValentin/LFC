#include "AFN.h"


void read(AFN &afn)
{
	std::ifstream file("input.txt");
	file >> afn;
	if (afn.verificare())
		afn.afisare();
	else
	{
		std::cout << "Automatul nu este corect!";
		exit(1);
	}

}

void message(bool &ok)
{
	std::string read;
	std::cout <<std::endl<< "Doriti sa introduceti un cuvant?(Da/Nu)";
	std::cin >> read;
	if (read == "da" || read == "DA" || read == "Da" || read == "dA")
		ok = true;
	else
		ok = false;
}

int main()
{
	std::string readWord;
	bool ok;
	AFN afn;
	read(afn);
	do
	{
		message(ok);
		if (ok == true)
		{
			std::cout << "Introduceti cuvantul: ";
			std::cin >> readWord;
			std::cout << "\n";
			if (afn.acceptat(readWord) == 1)std::cout << "Cuvantul este acceptat";
			else
				std::cout << "Cuvantul nu este acceptat!";
		}

	} while (ok);

}