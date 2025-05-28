///************************* ITELMA SP ****************************************

#include "ccppfunctiondeclarationparser.hpp"
#include <regex>

sParserResult cCppFunctionDeclarationParser::parse(std::string s)
{
  s = processSpaces(s); // remove reduntand spaces 

  return sParserResult();
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

  return s;
}