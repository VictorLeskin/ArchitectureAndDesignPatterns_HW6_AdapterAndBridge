///************************* ITELMA SP ****************************************
#ifndef CCPPFUNCTIONDECLARATIONPARSER_HPP
#define CCPPFUNCTIONDECLARATIONPARSER_HPP

#include <string>

struct sParserResult
{
  std::string sReturn;
  std::string sName;
  std::string sParameters;
  std::string sTailAttributes;
};

class cCppFunctionDeclarationParser
{
public:
  cCppFunctionDeclarationParser() {}

  sParserResult parse( std::string s );

protected:
  std::string processSpaces(std::string s);
};

#endif //#ifndef CCPPFUNCTIONDECLARATIONPARSER_HPP
