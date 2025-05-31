///************************* ITELMA SP ****************************************
#ifndef CCPPFUNCTIONDECLARATIONPARSER_HPP
#define CCPPFUNCTIONDECLARATIONPARSER_HPP

#include <string>
#include <vector>
#include "cexception.hpp"

struct sParserResult
{
	std::string sFunctionDeclaration;
	std::string sReturn;
	std::string sName;
	std::string sParameters;
	std::string sTailAttributes;

	std::vector<std::pair<std::string, std::string>> parameters; // тип и имя параметра
};

class cCppFunctionDeclarationParser
{
public:
	cCppFunctionDeclarationParser() {}

	sParserResult parse(std::string s);

protected:
	std::string processSpaces(std::string s);
	void split0(const std::string& s);

	void splitParameters();
	void createDerivedClassFunctionDeclaration();
	void createDCFD() { createDerivedClassFunctionDeclaration(); } // alias

	std::string createDCFDReturn();
	std::string createDCFDTailAttributes();

	std::string composeDCFD();

protected:
	sParserResult r;
	sParserResult derived;
};

#endif //#ifndef CCPPFUNCTIONDECLARATIONPARSER_HPP
