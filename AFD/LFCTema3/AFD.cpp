#include "AFD.h"

AFD::AFD()
{
	m_sigma = std::string::npos;
	m_stareInit= std::string::npos;
}

AFD::AFD(std::vector<std::string> stari, std::string sigma, std::vector<std::tuple<std::string, char, std::string>> tranzitie, std::string stareInit, std::vector<std::string> stariFinal)
{
	this->m_stari = stari;
	this->m_sigma = sigma;
	this->m_tranzitie = tranzitie;
	this->m_stariFinal = stariFinal;
	this->m_stareInit = stareInit;
}

bool AFD::isTerminal(std::string stare)
{
		for (int i = 0; i < m_stariFinal.size(); i++)
		{
			if (stare == m_stariFinal[i]) return true;
		}
		return false;

}

bool AFD::verificare()
{
	std::unordered_set <std::string> exist;
	std::unordered_set<char> existSigma;

	for (int i = 0; i < m_stari.size(); i++)
		exist.insert(m_stari[i]);

	for (int i = 0; i < m_sigma.size(); i++)
		existSigma.insert(m_sigma[i]);

	if (exist.find(m_stareInit)== exist.end()) return false;

	for(int i =0;i<m_stariFinal.size();i++)
		if (exist.find(m_stariFinal[i]) == exist.end()) return false;

	for (int i = 0; i < m_tranzitie.size(); i++)
	{
		auto a = m_tranzitie[i];
		if (exist.find(std::get<0>(a)) == exist.end()) return false;
		if (exist.find(std::get<2>(a)) == exist.end()) return false;
		if (existSigma.find(std::get<1>(a)) == existSigma.end()) return false;

	}

	return true;
}






int AFD::accepta(std::string word)
{
	std::string stare = m_stareInit;
	int j = 0;
	bool found = false;
	while (!isTerminal(stare))
	{
		found = false;
		for (int i = 0; i < m_tranzitie.size(); i++)
		{
			auto a = m_tranzitie[i];

			if (stare == std::get<0>(a) && word[j] == std::get<1>(a))
			{
				stare = std::get<2>(a);
				j++;
				if (j == word.length())
					if (isTerminal(stare))
						return 1;
					else
						return 0;
				found = true;
			}
			if (found == false && i == m_tranzitie.size() - 1)
			{

				return -1;
			}

		}

	}
}

std::istream& operator>>(std::istream& input, AFD& Obj)
{
	int size;
	input >> size;
	std::string aux1,aux3;
	char aux2;
	for (int i = 0; i < size; i++)
	{
		input >> aux1;
		Obj.m_stari.push_back(aux1);
	}

	input >> Obj.m_sigma;

	input >> size;
	for (int i = 0; i < size; i++)
	{
		input >> aux1>>aux2>>aux3;
		auto a = std::make_tuple(aux1, aux2, aux3);
		Obj.m_tranzitie.push_back(a);
	}
	input >> Obj.m_stareInit;
	input >> size;
	for (int i = 0; i < size; i++)
	{
		input >> aux1;
		Obj.m_stariFinal.push_back(aux1);
	}




	return input;
}

std::ostream& operator<<(std::ostream& output, AFD const& Obj)
{
	output << "M=({";
	for (int i = 0; i < Obj.m_stari.size(); i++)

		output << Obj.m_stari[i]<<" ";

	output << "},{"<<Obj.m_sigma<<"}, delta , "<<Obj.m_stareInit<<"  {";

	for (int i = 0; i < Obj.m_stariFinal.size(); i++)

		output << Obj.m_stariFinal[i] << " ";
	output << "})" << std::endl << "Delta = " << std::endl;

	output << "__|_";
	for (int i = 0; i < Obj.m_sigma.size(); i++)
		output << "_| " << Obj.m_sigma[i] << " |_";

	int k = 0;
	for (int i = 0; i < Obj.m_stari.size(); i++)
	{
		output << std::endl;
		output << Obj.m_stari[i] << "|_";
		
		if(k<Obj.m_tranzitie.size())
		for (int j = 0; j < Obj.m_sigma.size(); j++)
		{
			if (std::get<1>(Obj.m_tranzitie[k]) == Obj.m_sigma[j])
			{
				output << "_| " << std::get<2>(Obj.m_tranzitie[k]) << "|_";
				k++;
			}
			else
				output << "_| " << "-" << " |_";
		}
		else
			for (int j = 0; j < Obj.m_sigma.size(); j++)
			{
				
					output << "_| " << "-" << " |_";
			}
	}
	output << std::endl;
	return output;
}
