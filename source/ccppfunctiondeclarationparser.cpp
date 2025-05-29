///************************* ITELMA SP ****************************************

#include "ccppfunctiondeclarationparser.hpp"
#include <regex>

sParserResult cCppFunctionDeclarationParser::parse(std::string s)
{
  r = sParserResult();

  s = processSpaces(s); // remove reduntand spaces 

  return sParserResult();
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
  const std::string s = r.sParameters;

  std::regex paramRegex(R"((\w+(?:\s*::\s*\w+|\s*\*\s*|\s*&\s*)*)\s*(\w*)\s*,?)");

  std::string& paramsStr = r.sParameters;
  if (!paramsStr.empty()) 
  {
    std::sregex_iterator paramIt(paramsStr.begin(), paramsStr.end(), paramRegex);
    std::sregex_iterator end;

    while (paramIt != end) 
    {
      std::smatch paramMatch = *paramIt;
      std::string paramType = paramMatch[1].str();
      std::string paramName = paramMatch[2].str();

      if (!paramType.empty()) 
        r.parameters.emplace_back(paramType, paramName);
      ++paramIt;
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