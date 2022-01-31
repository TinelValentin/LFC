#include "AFD.h"

#include <fstream>

int main()
{
	AFD input;
	std::ifstream fis("input.txt");
	fis >> input;
	if (input.verificare())
	{
		std::cout << input;
		input.minimizare();
	}
	else
		std::cout << "Afd incorect!";
}