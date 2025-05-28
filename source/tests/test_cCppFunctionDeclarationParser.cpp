///************************* ITELMA SP ****************************************

#include <gtest/gtest.h>

#include "ccppfunctiondeclarationparser.hpp"

// clang-format off

// gTest grouping class
class test_cCppFunctionDeclarationParser : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_cCppFunctionDeclarationParser : public cCppFunctionDeclarationParser
  {
  public:
    // add here members for free access.
    using cCppFunctionDeclarationParser::cCppFunctionDeclarationParser; // delegate constructors
    using cCppFunctionDeclarationParser::processSpaces;
  };

};
 
TEST_F(test_cCppFunctionDeclarationParser, test_ctor )
{
  Test_cCppFunctionDeclarationParser t;
}

TEST_F(test_cCppFunctionDeclarationParser, test_processSpaces)
{
  {
    Test_cCppFunctionDeclarationParser t;
    std::string decl = "  virtual int \t   foo( const std::string& s,  int * ptr,  double   d  ) const  =  0    ";

    std::string r = t.processSpaces(decl);
    EXPECT_EQ(r, "virtual int foo(const std::string& s, int * ptr, double d)const = 0");
  }
}

