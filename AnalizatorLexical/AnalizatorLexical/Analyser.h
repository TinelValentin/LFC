#pragma once
#include <fstream>
#include <regex>
#include <iostream>
#include <unordered_map>
#include<string>
#include "AFD.h"
class Analyser
{
	std::unordered_multimap<std::string, std::string> tokens;
	AFD identificator, math, operators, number, paranthesis, symbols,error;
	std::unordered_map<std::string, int> keywords;
	public:


		bool checkName(std::string id);
		void identifyWord(std::string &id,int lineNumber);
		void readKeywords();
		void checkParanthesis(std::string id, int& bracket, int& cBracket);
		void clearString(std::string& id);
		int identifyString(std::string id);
		int checkToken(std::string& id,int &bracket,int &cBracket,bool &braket);
		void checkLine(std::string line, int &bracket, int &cBracket);
		void read();
		
};

