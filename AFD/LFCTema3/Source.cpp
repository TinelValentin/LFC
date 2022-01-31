#include "AFD.h"
#include <fstream>

void read(AFD& test)
{
	std::ifstream fis("input.txt");
	fis >> test;
	if (!test.verificare())
	{
		std::cout << "Automatul nu este corect!";
		exit(1);
	}
	else
	std::cout << test;
	fis.close();
}

int main()
{
	AFD test;
	read(test);
	bool ok = true;
	std::string aux;
	int8_t answer;
	std::cout << std::endl;
	while (ok)
	{
		std::cout << "Introduceti un cuvant: ";
		std::cin >> aux;
		answer = test.accepta(aux);
		if (answer == 1)
			std::cout << "Cuvantul este acceptat!";
		else
			if (answer == 0)
				std::cout << "Cuvantul nu este acceptat!";
			else
				if (answer == -1)
				std::cout << "Blocaj!";

		std::cout << std::endl<<"Doriti sa mai introduceti un cuvant?(Da/Nu)"<< std::endl;
		std::cin >> aux;
		if (aux == "DA" || aux == "da" || aux == "Da" || aux == "dA")
			ok = true;
		else
			ok = false;
		
	}
	
}