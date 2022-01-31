#pragma once

#include<iostream>
#include<cstdint>
#include <vector>
#include <string>
#include<tuple>
#include <unordered_set>
class AFD
{
private:
	std::vector<std::string> m_stari;
	std::string m_sigma;
	std::vector<std::tuple <std::string, char, std::string>> m_tranzitie;
	std::vector<std::string> m_stariFinal;
	std::string m_stareInit;
public:
	AFD();
	AFD(std::vector<std::string> stari, std::string sigma, std::vector<std::tuple<std::string, char, std::string>> tranzitie, std::string stareInit, std::vector<std::string> stariFinal);

	bool isTerminal(std::string stare);
	bool verificare();
	int accepta(std::string);

	friend std::istream& operator >> (std::istream& input, AFD& Obj);
	friend std::ostream& operator << (std::ostream& output, AFD const &Obj);

};

