#include "AFN.h"

std::istream& operator>>(std::istream& input, AFN& object)
{
	int size;
	std::string read,readSecond, readThird;
	char readChar;
	input >> size;
	for (int i = 0;i < size;i++)
	{
		input >> read;
		object.m_stari.push_back(read);
	}

	input >> size;
	for (int i = 0; i < size; i++)
	{
		input >> readChar;
		object.m_sigma.push_back(readChar);
	}

	input >> size;
	for (int i = 0; i < size; i++)
	{
		input >> read;
		input >> readChar;
		input >> readThird;
		auto pair = std::make_pair(readChar, readThird);
		object.m_transitions.insert(make_pair(read, pair));
	}

	input >> object.m_stare_init;

	input >> size;
	for (int i = 0; i < size; i++)
	{
		input >> read;
		object.m_stariFinale.push_back(read);
	}

	return input;
}

void AFN::afisare()
{
	int max = 0;
	for (int i = 0; i < m_stari.size(); i++)
	{
		int number = m_transitions.count(m_stari[i]);
		if (number > max) max = number;
	}
	std::cout << "|----------";
	for (int i = 0; i < m_sigma.size(); i++)
	{
		for (int j = 0; j <= 2 * max; j++)
			std::cout << "--";
		std::cout << "-";
	}
	std::cout << "-";

	std::cout << "\b" << "|";
	std::cout << std::endl;
	std::cout << "|          ";
	for (int i = 0; i < m_sigma.size(); i++)
	{
		std::cout << "|";
		for (int j = 0; j < max ; j++)
			std::cout << "  ";
		std::cout  << m_sigma[i] ;
		for (int j = 0; j < max ; j++)
			std::cout << "  ";
		std::cout << "|";
		std::cout << "\b" << " ";
	}
	std::cout << " ";
	std::cout << "\b" << "|";
	std::cout << std::endl;
	std::cout << "|----------";
	for (int i = 0; i < m_sigma.size(); i++)
	{
		for (int j = 0; j <= 2 * max; j++)
			std::cout << "--";
		std::cout << "-";
	}
	std::cout<<"-" << "\b" << "|";
	std::cout << std::endl;

	



	for (int i = 0; i < m_stari.size(); i++)
	{
		auto transitions = m_transitions.equal_range(m_stari[i]);
		std::cout << "|    "<< m_stari[i]<<"    ";
		for (int i = 0; i < m_sigma.size(); i++)
		{
			int numberOfElements = 0;
			std::cout << "|{";
			for (auto it = transitions.first; it != transitions.second; it++)
				if (it->second.first == m_sigma[i])
				{
					std::cout << it->second.second;
					numberOfElements++;
				}
					
			for(int j=numberOfElements;j<2*max;j++)
			std::cout<< "  ";
			std::cout << "}";
		
		}
		std::cout  << "|";
		std::cout << std::endl;
	}
	std::cout << "|----------";
	for (int i = 0; i < m_sigma.size(); i++)
	{
		for (int j = 0; j <= 2 * max; j++)
			std::cout << "--";
		std::cout << "-";
	}
	std::cout << "-" << "\b" << "|";
}

bool AFN::verificare()
{
	std::unordered_set<std::string> stari;
	std::unordered_set<char> sigmas;
	for (int i = 0; i < m_stari.size(); i++)
		stari.insert(m_stari[i]);
	for (int i = 0; i < m_sigma.size(); i++)
		sigmas.insert(m_sigma[i]);

	for (auto i : m_transitions)
	{
		std::string key = i.first;
		char sigmaTransition = i.second.first;
		std::string final = i.second.second;

		if (stari.find(key) == stari.end())return false;
		if (stari.find(final) == stari.end()) return false;
		if (sigmas.find(sigmaTransition) == sigmas.end()) return false;

	}

	if (stari.find(m_stare_init) == stari.end())return false;

	for (int i = 0; i < m_stariFinale.size(); i++)
		if (stari.find(m_stariFinale[i]) == stari.end())return false;

	return true;

}

bool AFN::isTerminal(std::string stare)
{
	for (int i = 0; i < m_stariFinale.size(); i++)
		if (stare == m_stariFinale[i])
			return true;
	return false;
}


int AFN::acceptat(std::string word)
{
	int j = 0;
	std::vector<std::pair<int,std::string>> controlStare;
	std::string stare = m_stare_init;
	bool found = false;
	controlStare.push_back(std::make_pair(0,stare));
	while (j<word.length())
	{
		found = false;
		int numberTransition = 0;
		auto tranzitie = m_transitions.equal_range(stare);
		int numarDeStari = m_transitions.count(stare);
		if(numarDeStari>0)
		for (auto it = tranzitie.first; it != tranzitie.second; it++)
		{
			numberTransition++;
			
			if (it->second.first == word[j]&&controlStare.back().first<numberTransition)
			{
				controlStare.back().first = numberTransition;   

				stare = it->second.second;
				j++;
				found = true;
				if(j<word.length())
				controlStare.push_back(std::make_pair(0,stare));
				break;
			}

		}
		else
		{
			controlStare.pop_back();
			stare = controlStare.back().second;
			j--;
		}
		if (found == false)
			if (controlStare.size() > 1)
			{
				controlStare.pop_back();
				stare = controlStare.back().second;
				j--;
			}
			else
				return 0;
			
		if (controlStare.size() == 0) return 0;

		if (j == word.length())
			if (isTerminal(stare))return 1;
			else
				if (controlStare.back().first < numarDeStari) 
				{
					
					stare = controlStare.back().second;
					j--;
				}
				
				else
					if (controlStare.size() > 1)
					{
						controlStare.pop_back();
						stare = controlStare.back().second;
						j -= 2;
					}
				else return 0;
	}
	return 1;

}
