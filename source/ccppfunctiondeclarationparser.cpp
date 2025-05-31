///************************* ITELMA SP ****************************************

#include "ccppfunctiondeclarationparser.hpp"
#include <regex>
#include <string>
#include <sstream>

sParserResult cCppFunctionDeclarationParser::parse(std::string s)
{
  r.sFunctionDeclaration = s;

  r = sParserResult();

  s = processSpaces(s); // remove reduntand spaces 
  split0(s);
  splitParameters();

  return r;
}

void cCppFunctionDeclarationParser::split0(const std::string& s)
{
  std::regex funcRegex(R"((.*)\s(\w+)\((.*)\)(.*))");
  std::smatch matches;

  if (std::regex_match(s, matches, funcRegex))
  {
    r.sReturn = matches[1].str(); // return 
    r.sName = matches[2].str(); // name
    r.sParameters = matches[3].str(); // parameters
    r.sTailAttributes = matches[4].str(); // tail (const)
  }
  else
    throw(cException("The string doesn't match the adaptee function declaration."));
}

void cCppFunctionDeclarationParser::splitParameters()
{
  std::istringstream strm(r.sParameters);
  std::string s;
  while (std::getline(strm, s, ','))
  {
      std::regex nameRegex(R"((.*) (\w+)$)"); 
      std::smatch matches;
      if (std::regex_match(s, matches, nameRegex))
      {
          std::string paramType = matches[1].str();
          std::string paramName = matches[2].str();
          r.parameters.emplace_back(paramType, paramName);
      }
  }
}


std::string cCppFunctionDeclarationParser::processSpaces(std::string s)
{
  // remove extra spaces
  s.erase(0, s.find_first_not_of(" \t\n\r")); // Remove leading whitespace
  s.erase(s.find_last_not_of(" \t\n\r") + 1); // Remove trailing whitespace
  auto repl = [&](const char* reg, const char* r)
  {
    s = std::regex_replace(s, std::regex(reg), r);
  };

  repl(  "\t", " "); // Replace tabs
  repl("\\s+", " "); // Replace all matches with a single space
  repl("\\( ", "("); // Remove space around () = ;
  repl(" \\(", "(");
  repl("\\) ", ")");
  repl(" \\)", ")");
  repl(" ,", ",");
  repl(", ", ",");

  return s;
}

std::string cCppFunctionDeclarationParser::createDCFDReturn()
{
    return std::regex_replace(r.sReturn, std::regex(R"(virtual\s*)"), "");
}

std::string cCppFunctionDeclarationParser::createDCFDTailAttributes()
{
    std::string ret = std::regex_replace(r.sTailAttributes, std::regex(R"(\s*=\s*0)"), "");
    return std::regex_replace(ret, std::regex(R"(\s*;)"), "");
}



void cCppFunctionDeclarationParser::createDerivedClassFunctionDeclaration()
{
    derived = r;
    derived.sReturn = createDCFDReturn();
    derived.sName = r.sName;
    derived.parameters = r.parameters;
    derived.sTailAttributes = createDCFDTailAttributes();

    derived.sFunctionDeclaration = composeDCFD();
}

std::string cCppFunctionDeclarationParser::composeDCFD()
{
    std::ostringstream strm;
    strm << derived.sReturn
        << " " << derived.sName
        << "(";
    for( int i = 0; i < derived.parameters.size(); ++i )
    {
        const auto& p = derived.parameters[i];
        if (i != 0) strm << ",";
        strm << p.first << " " << p.second;
    }
    strm << ")"
        << derived.sTailAttributes
        << " override";

    return strm.str();
}
