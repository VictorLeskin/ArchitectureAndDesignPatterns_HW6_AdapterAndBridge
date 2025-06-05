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

	const std::string& ClassName() const { return className; }
	void setClassName(const std::string& cn) { className = cn; }
	const sParserResult& ParseResult() const { return r; }

protected:
	std::string processSpaces(std::string s);
	void split0(const std::string& s);
	
	void splitParameters();

protected:
	std::string className;
	sParserResult r;
};

class cAddapterCppFunctionDeclarationTransformer
{
public:
	cAddapterCppFunctionDeclarationTransformer() {}

	sParserResult transform(const sParserResult& r)
	{
		createDerivedClassFunctionDeclaration(r);
		return derived;
	}

	const std::string& ClassName() const { return className; }
	void setClassName(const std::string& cn) { className = cn; }


protected:
	void createDerivedClassFunctionDeclaration(const sParserResult& r);
	void createDCFD(const sParserResult& r) { createDerivedClassFunctionDeclaration(r); } // alias

	std::string createDCFDReturn();
	std::string createDCFDTailAttributes();

	std::string composeDCFD();
	std::string composeDerivedClassFunctionBody();
	std::string composeDCFB() { return composeDerivedClassFunctionBody(); }
	std::string composeDCFB_set() const;
	std::string composeDCFB_get() const;


protected:
	std::string className;
	sParserResult derived;
};




#endif //#ifndef CCPPFUNCTIONDECLARATIONPARSER_HPP
