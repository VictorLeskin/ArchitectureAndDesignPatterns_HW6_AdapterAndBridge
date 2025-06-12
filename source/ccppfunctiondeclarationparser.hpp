///************************* ITELMA SP ****************************************
#ifndef CCPPFUNCTIONDECLARATIONPARSER_HPP
#define CCPPFUNCTIONDECLARATIONPARSER_HPP

#include <string>
#include <vector>
#include <memory>
#include "cexception.hpp"


class cInterfaceClass;
class cAdapterClass;


struct sParserResult
{
	std::string sFunctionDeclaration;
	std::string sReturn;
	std::string sName;
	std::string sParameters;
	std::string sTailAttributes;

	std::vector<std::pair<std::string, std::string>> parameters; // тип и имя параметра
};

struct sDerivedClassFunction : public sParserResult
{
	std::string tBody;
	std::string Body() const
	{
		return   std::string("{\n")
				+ std::string("    ") + tBody + "\n"
				+ std::string("}\n");
	}
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

	std::shared_ptr<cAdapterClass> createAdapterClass(const cInterfaceClass* ic) const;

protected:
	void setClassName(const std::string&) const;
	std::string createClassName(const std::string &) const;
	sDerivedClassFunction createDerivedClassFunction(const sParserResult &) const;

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
	mutable std::string className;
	sParserResult derived;
};




#endif //#ifndef CCPPFUNCTIONDECLARATIONPARSER_HPP
