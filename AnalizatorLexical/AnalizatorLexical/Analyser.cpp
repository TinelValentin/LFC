#include "Analyser.h"


void Analyser::readKeywords()
{
	keywords.insert(std::make_pair("for", 1));
	keywords.insert(std::make_pair("while", 1));
	keywords.insert(std::make_pair("if", 1));
	keywords.insert(std::make_pair("else", 2));
	keywords.insert(std::make_pair("int", 3));
	keywords.insert(std::make_pair("float", 3));
	keywords.insert(std::make_pair("bool", 3));
	keywords.insert(std::make_pair("double", 3));

}

void readAutomat(AFD& automat,std::string fileName)
{
	std::ifstream file(fileName);
	file >> automat;
	if (!automat.verificare())
		std::cout << "automat gresit!";
}

void Analyser::checkParanthesis(std::string id,int &bracket,int &cBracket)
{
	if (id == "(")
	{
		bracket++;
		std::cout << "(" << "token_BO" << "," << "_" << "), ";
		tokens.insert(std::make_pair("token_BO", "_"));
	}
	else
	if (id == ")")
	{
		bracket++;
		std::cout << "(" << "token_BC" << "," << "_" << "), ";
		tokens.insert(std::make_pair("token_BC", "_"));
	}
	else
	if (id == "{")
	{
		cBracket++;
		std::cout << "(" << "token_CBO" << "," << "_" << "), ";
		tokens.insert(std::make_pair("token_CBO", "_"));
	}
	else
	if (id == "}")
	{
		std::cout << "(" << "token_CBC" << "," << "_" << "), ";
		tokens.insert(std::make_pair("token_CBC", "_"));
		cBracket--;
	}
	
}


void Analyser::clearString(std::string &id)
{
	std::string copy;
	copy = id[id.length()-1];
	id.clear();
	if(copy!=" ")
		id = copy;
}

int Analyser::identifyString(std::string id)
{
	id.pop_back();
	if (keywords.find(id) != keywords.end())

		return keywords.at(id);
	else
	return 4;
}

int Analyser::checkToken(std::string& id,int &bracket,int &Cbracket,bool & braket)
{
	
	std::string show;
	if (identificator.accepta(id) == 1)
	{
		
		switch (identifyString(id))
		{
		case 1:
			//for while
			show = id;
			show.pop_back();
			std::cout << "(" << "token_Keyword" << "," << show << "), ";
			tokens.insert(std::make_pair("token_Keyword", show));
			clearString(id);
			return 4;
			break;
		case 2:
			//int float
			show = id;
			show.pop_back();
			std::cout << "(" << "token_ID_" + show << "," << "_" << "), ";
			tokens.insert(std::make_pair("token_ID_"+show, "_"));
			break;

		case 3:  //else
			show = id;
			show.pop_back();
			std::cout << "(" << "token_Keyword" << "," << show << "), ";
			tokens.insert(std::make_pair("token_Keyword", show));
			break;
				
		default: //name
			show = id;
			show.pop_back();
			std::cout << "(" << "token_ID_" << "," << "pointer_la_" + show << "), ";
			tokens.insert(std::make_pair("token_ID_", "pointer_la_" + show));
			break;
		}

		clearString(id);
		if (braket)
		{
			return 5;
		}
	}
	
		if (number.accepta(id) == 1)
		{
			show = id;
			show.pop_back();
			if (show.length() > 255)
				return 3;
			std::cout << "(" << "token_Number" << "," << show << "), ";
			tokens.insert(std::make_pair("token_Number", show));
			clearString(id);
			if (braket)
				return 5;
		}
		
			if (math.accepta(id) == 1)
			{
				show = id;
				show.pop_back();
				std::cout << "(" << "token_math" << "," << show << "), ";
				tokens.insert(std::make_pair("token_math", show));
				clearString(id);
				if (braket)
					return 5;
			}
			
				if (operators.accepta(id) == 1)
				{
					show = id;
					show.pop_back();
					std::cout << "(" << "token_operator" << "," << show << "), ";
					tokens.insert(std::make_pair("token_operator", show));
					clearString(id);
					if (braket)
						return 5;
				}
				
					if (paranthesis.accepta(id) == 1)
					{
						show = id;
						show.pop_back();
						checkParanthesis(show, bracket, Cbracket);
						clearString(id);
					}
					
						if (symbols.accepta(id) == 1)
						{
							show = id;
							show.pop_back();
							if (show == "//")
							{
								id.clear();
								return 1;
							}
							std::cout << "(" << "token_symbol" << "," << show << "), ";
							clearString(id);
							if (show == "\"")
							{	
								return 2;
							}
							if (braket)
								return 5;
							
								
						}
						
							if (error.accepta(id) == 1)
							{
								std::cout << "(" << "token_error" << "," << "_" << "), ";
								clearString(id);
								if (braket)
									return 5;
							}
	return 0;
}


void Analyser::checkLine(std::string line,int &bracket,int &cBracket)
{

	static std::string id;
	std::string show;
	bool string = false;
	bool braket = false;
    
	for (int i = 0; i < line.length(); i++)
	{
		if (id[0] == ' ')
			id.erase(0,1);
		id += line[i];
		int message = checkToken(id, bracket, cBracket,braket);
		switch (message)
		{
		case 1:
			return;
			break;
		case 2:
			string = !string;
			break;
		case 3:
			std::cout << "(" << "token_Number_ERROR" << "," << "constant too long" << "), ";
			break;
		case 4:
			bracket = true;
			break;
		case 5:
			std::cout << "(" << "token_ERROR" << "," << "Bracket needed" << "), ";
		default:
			break;
		}
		
	}
	
	
	checkToken(id,bracket,cBracket, braket);

	if(string==true)
		std::cout << "(" << "token_QUOTE_ERROR" << "," << "quotes not closed" << "), ";

	
}

	void Analyser::read()
	{
		readKeywords();
		int bracket = 0, cBracket = 0;
		std::ifstream file("input.txt");
		bool commentary = false;
		std::string id,line;
		readAutomat(identificator, "afdIdentificator.txt");
		readAutomat(number, "afdCifre.txt");
		readAutomat(math, "afdMath.txt");
		readAutomat(operators, "afdOperator.txt");
		readAutomat(paranthesis, "afdParanthesis.txt");
		readAutomat(symbols, "afdElse.txt");
		readAutomat(error, "error.txt");
		while (!file.eof())
		{
			std::getline(file, line);
			if(line!="")
			checkLine(line,bracket,cBracket);

		

		}
		checkLine("  ",bracket,cBracket);

		
		if (bracket != 0)
			std::cout << "\n" << "(" << "token_error" << "," << " Bracket Error" << "),";
		if (cBracket != 0)
			std::cout << "\n" << "(" << "token_error" << "," << " Curly Bracket Error" << "),";

		
	}




