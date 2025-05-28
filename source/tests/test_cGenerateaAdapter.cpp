///************************* ITELMA SP ****************************************

#include <gtest/gtest.h>

#include "cgenerateaadapter.hpp"
#include "ispaceship.hpp"


// clang-format off

// gTest grouping class
class test_cGenerateaAdapter : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_cGenerateaAdapter : public cGenerateaAdapter
  {
  public:
    // add here members for free access.
    using cGenerateaAdapter::cGenerateaAdapter; // delegate constructors
  };

};
 
TEST_F(test_cGenerateaAdapter, test_ctor )
{
  //Test_cGenerateaAdapter t;
}


#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

struct Functiondecl {
  string returnType;
  string functionName;
  vector<pair<string, string>> parameters; // тип и имя параметра
  bool isConst;
};

Functiondecl parseFunctiondecl(const string& decl) {
  Functiondecl result;
  result.isConst = false;

  // Регулярные выражения для разбора
  regex funcRegex(R"((\w+(?:\s*::\s*\w+|\s*\*\s*|\s*&\s*)*)\s+(\w+)\s*\((.*)\)\s*(const)?\s*)");
  regex paramRegex(R"((\w+(?:\s*::\s*\w+|\s*\*\s*|\s*&\s*)*)\s*(\w*)\s*,?)");

  smatch matches;
  if (regex_match(decl, matches, funcRegex)) {
    // Возвращаемый тип
    result.returnType = matches[1].str();

    // Имя функции
    result.functionName = matches[2].str();

    // Параметры функции
    string paramsStr = matches[3].str();
    if (!paramsStr.empty()) {
      sregex_iterator paramIt(paramsStr.begin(), paramsStr.end(), paramRegex);
      sregex_iterator end;

      while (paramIt != end) {
        smatch paramMatch = *paramIt;
        string paramType = paramMatch[1].str();
        string paramName = paramMatch[2].str();

        if (!paramType.empty()) {
          result.parameters.emplace_back(paramType, paramName);
        }
        ++paramIt;
      }
    }

    // Проверка на const
    if (matches.size() > 4 && !matches[4].str().empty()) {
      result.isConst = true;
    }
  }
  else {
    throw runtime_error("Invalid function decl format");
  }

  return result;
}

std::string strip(std::string s) 
{
  // Remove leading whitespace
  s.erase(0, s.find_first_not_of(" \t\n\r"));

  // Remove trailing whitespace
  s.erase(s.find_last_not_of(" \t\n\r") + 1);
  return s;
}

std::string whitespaces_replace(std::string s)
{
  // Regex to match one or more whitespace characters
  std::regex whitespace_regex("\\s+");

  // Replace all matches with a single space
  return std::regex_replace(s, whitespace_regex, " ");
}

TEST_F(test_cGenerateaAdapter, test_parse_function_delaration )
{
  string decl = "  virtual int \t   foo( const std::string& s,  int * ptr, double d = 3.14 ) const = 0";

  // remove extra spaces
  decl.erase(0, decl.find_first_not_of(" \t\n\r")); // Remove leading whitespace
  decl.erase(decl.find_last_not_of(" \t\n\r") + 1); // Remove trailing whitespace
  decl = std::regex_replace(decl, std::regex("\t"), " "); // Replace tabs
  decl = std::regex_replace(decl, std::regex("\\s+"), " "); // Replace all matches with a single space
  decl = std::regex_replace(decl, std::regex("\\( "), "("); // Remove space around (, ) = ;
  decl = std::regex_replace(decl, std::regex(" \\("), "(");
  decl = std::regex_replace(decl, std::regex("\\) "), ")");
  decl = std::regex_replace(decl, std::regex(" \\)"), ")");
  decl = std::regex_replace(decl, std::regex(" ;"), ";");
  decl = std::regex_replace(decl, std::regex(" ="), "=");
  decl = std::regex_replace(decl, std::regex("= "), "=");
  decl = std::regex_replace(decl, std::regex(" ,"), ",");
  decl = std::regex_replace(decl, std::regex(", "), ",");
  decl = std::regex_replace(decl, std::regex(" &"), "&");
  decl = std::regex_replace(decl, std::regex("& "), "&");
  decl = std::regex_replace(decl, std::regex("\\* "), "*");
  decl = std::regex_replace(decl, std::regex(" \\*"), "*");

    try {

    Functiondecl parsed = parseFunctiondecl(decl);

    cout << "Return type: " << parsed.returnType << endl;
    cout << "Function name: " << parsed.functionName << endl;
    cout << "Parameters:" << endl;
    for (const auto& param : parsed.parameters) {
      cout << "  " << param.first << " " << param.second << endl;
    }
    cout << "Is const: " << (parsed.isConst ? "yes" : "no") << endl;
  }
  catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
  }
}
