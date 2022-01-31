#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <unordered_set>
class AFN
{
private:

	std::vector<std::string> m_stari;
	std::unordered_multimap<std::string, std::pair<char, std::string>> m_transitions;
	std::string m_sigma;
	std::vector<std::string> m_stariFinale;
	std::string m_stare_init;


public:
	friend std::istream& operator>> (std::istream& input, AFN& object);
	void afisare();
	bool verificare();
	bool isTerminal(std::string stare);
	int acceptat(std::string);

};

