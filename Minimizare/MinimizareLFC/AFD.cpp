#include "AFD.h"

struct pair_hash {
	
	std::size_t operator () (const std::pair<std::string, char>& p) const {
		auto h1 = std::hash<std::string>{}(p.first);
		auto h2 = std::hash<char>{}(p.second);

	
		return h1 ^ h2;
	}
};

AFD::AFD()
{
	m_sigma = std::string::npos;
	m_stareInit = std::string::npos;
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

	if (exist.find(m_stareInit) == exist.end()) return false;

	for (int i = 0; i < m_stariFinal.size(); i++)
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

void AFD:: updateStari(std::unordered_map<std::string, bool> visited)
{
	for (auto i : visited)

		if (i.second == false)
		{
			for (int j = 0; j < m_stari.size(); j++)
				if (m_stari[j] == i.first)
					m_stari.erase(m_stari.begin() + j);


			for (int j = 0; j < m_tranzitie.size(); j++)
			{
				if (std::get<0>(m_tranzitie[j]) == i.first)
				{
					m_tranzitie.erase(m_tranzitie.begin() + j);
				}
			}

			for (int j = 0; j < m_stariFinal.size(); j++)
			{
				if (m_stariFinal[j] == i.first)
				{
					m_stariFinal.erase(m_stariFinal.begin() + j);
				}
			}
		}

}

void AFD::removeInaccessible()
{
	std::string currentState = m_stareInit;
	std::unordered_map<std::string, bool> visited;
	bool found;
	for (int i = 0; i < m_stari.size(); i++)
		visited.insert(std::make_pair(m_stari[i],false));
	std::stack<std::string> stariStiva;

	stariStiva.push(currentState);

	while (stariStiva.size() > 0)
	{
		visited[currentState] = true;
		found = false;
		for (int i = 0; i < m_tranzitie.size(); i++)
		{
			if(std::get<0>(m_tranzitie[i])==currentState)
				if (visited[std::get<2>(m_tranzitie[i])]==false)
				{
					found = true;
					currentState = std::get<2>(m_tranzitie[i]);
					stariStiva.push(currentState);
					break;

				}
		}
		if (found == false)
		{
			stariStiva.pop();
			if(stariStiva.size()>0)
			currentState = stariStiva.top();
		}
			
	}

	updateStari(visited);
}



std::string AFD::cautaTranzitie(std::string stare,char simbol)
{
	for (auto a : m_tranzitie)
	{
		if (std::get<0>(a) == stare && std::get<1>(a) == simbol)
			return std::get<2>(a);
	}
	return " " ; 
}

bool AFD::stareEchivalenta(std::vector<std::unordered_set<std::string>>&componente)
{
	std::unordered_set<std::string> clasa;
	
	bool change = false;
	int contor = -1;
	int size = componente.size();  

	for (int i = 0; i < size; i++)
	{
		contor++;
		clasa = componente[contor];
		if(clasa.size()>1)
		for (int j = 0; j <clasa.size();j++)   
		{
			auto iterator = clasa.begin();
			std::unordered_set<std::string> clasaNoua;
			clasaNoua.insert(*iterator);
			std::string copy = *iterator;
			
			 clasa.erase(iterator++);
			 iterator = clasaNoua.find(copy);  

			 for (auto k = clasa.begin(); k != clasa.end(); k++)
			{
				bool stareDiferita = false;
				for (int sigma = 0; sigma < m_sigma.size(); sigma++)
				{
					std::string stare1 = cautaTranzitie(*iterator,m_sigma[sigma]);
					std::string stare2 = cautaTranzitie(*k, m_sigma[sigma]);
					if (clasa.find(stare1) == clasa.end() && clasaNoua.find(stare1) == clasaNoua.end())
					{
						if (clasa.find(stare2) != clasa.end() && clasaNoua.find(stare2) != clasaNoua.end())

						{
							stareDiferita = true;
							change = true;
							break;
						}
						
					
					}
					else
					{
						if (clasa.find(stare2) == clasa.end() && clasaNoua.find(stare2) == clasaNoua.end())
						{
							stareDiferita = true;
							change = true;
							break;
						}
					}
					
					
				}
				if (stareDiferita == false)
				{
					clasaNoua.insert(*k);
						clasa.erase(k++);
						if(clasa.size()>1)
						k--;
					if (clasa.size() == 0) 
					{
						break;
					}
				}
				

			}
			 
			if(clasa.size()>0)
				componente[contor] = clasa;
			else
			{
				componente.erase(componente.begin() + contor);
				contor--;
				
			}
			
			 componente.push_back(clasaNoua);
		}
		
		change = false;
		
	}
	return change;
}


void AFD::transformToAFD(std::vector<std::unordered_set<std::string>> componente)
{
	std::ofstream output("output.txt");
	output << componente.size() << std::endl;
	for (int i = 0; i < componente.size(); i++)
	{
		output << "a" << i<<" ";
	}
	output << std::endl;
	output << m_sigma.size() << std::endl;
	for (int i = 0; i < m_sigma.size(); i++)
		output << m_sigma[i];
	output << std::endl;
	std::unordered_set < std::pair<std::string, char>,pair_hash> exists;
	
	
	int contorTranzitii;
	for (int i = 0; i < m_tranzitie.size(); i++)
	{
		std::string stareOutputFirst=" ", stareOutputSecond=" ";
		std::string stareInitiala = std::get<0>(m_tranzitie[i]);
		std::string stareFinala = std::get<2>(m_tranzitie[i]);
		int contor = -1;
		for (auto compCurent: componente)
		{
			contor++;
			if (compCurent.find(stareInitiala) != compCurent.end())stareOutputFirst = "a" + std::to_string(contor);
			if (compCurent.find(stareFinala) != compCurent.end())stareOutputSecond = "a" + std::to_string(contor);
		}
		if (stareOutputFirst != " " && stareOutputSecond != " ")
		{
			if (exists.find(make_pair(stareOutputFirst, std::get<1>(m_tranzitie[i]))) == exists.end())
			{
				output << stareOutputFirst << " " << std::get<1>(m_tranzitie[i]) << " " << stareOutputSecond << std::endl;
				exists.insert(make_pair(stareOutputFirst, std::get<1>(m_tranzitie[i])));
				
			}
			
		}
			
	}
	

	int contorInit = -1;

	for (auto compCurent : componente)
	{
		contorInit++;
		if (compCurent.find(m_stareInit) != compCurent.end())
			output << "a" << contorInit << " ";

	}

	output << std::endl;

	std::unordered_set < std::string> existsFinal;
	output << m_stariFinal.size() << std::endl;
	for (int i = 0; i < m_stariFinal.size(); i++)
	{
		int contor = -1;
		for (auto compCurent : componente)
		{
			contor++;
			if (compCurent.find(m_stariFinal[i]) != compCurent.end())  

				if (existsFinal.find("a"+std::to_string(contor)) == existsFinal.end())
				{
					existsFinal.insert("a"+ std::to_string(contor));
					output << "a" << contor << " ";
					

				}
				

		}
	}
	
}

void AFD::minimizare()
{
	removeInaccessible();
	std::vector<std::unordered_set<std::string>> componente = stariInceput();
	bool change;
	do
	{
		 change = false;
		change =stareEchivalenta(componente);
	} while (change == true);

	
	transformToAFD(componente);
	
	
	
}

std::vector<std::unordered_set<std::string>> AFD::stariInceput()
{
	std::vector<std::unordered_set<std::string>>componente;
	std::unordered_set<std::string> stariNefinale;
	std::unordered_set<std::string> stariFinale;
	
	for (auto a : m_stari)
	{
		stariNefinale.insert(a);
	}

	for (auto a : m_stariFinal)
	{
		stariFinale.insert(a);
		stariNefinale.erase(a);
		
	}
	 componente.push_back(stariNefinale);
	 componente.push_back(stariFinale);
	 return componente;
}

std::istream& operator>>(std::istream& input, AFD& Obj)
{
	int size;
	input >> size;
	std::string aux1, aux3;
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
		input >> aux1 >> aux2 >> aux3;
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

		output << Obj.m_stari[i] << " ";

	output << "},{" << Obj.m_sigma << "}, delta , " << Obj.m_stareInit << "  {";

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

		if (k < Obj.m_tranzitie.size())
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