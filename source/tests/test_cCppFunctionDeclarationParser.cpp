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
    using cCppFunctionDeclarationParser::split0;
    using cCppFunctionDeclarationParser::splitParameters;

    using cCppFunctionDeclarationParser::r;
  };

};

// gTest grouping class
class test_cAddapterCppFunctionDeclarationTransformer : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_cAddapterCppFunctionDeclarationTransformer : public cAddapterCppFunctionDeclarationTransformer
  {
  public:
    // add here members for free access.
    using cAddapterCppFunctionDeclarationTransformer::createDCFDReturn;
    using cAddapterCppFunctionDeclarationTransformer::createDCFDTailAttributes;
    using cAddapterCppFunctionDeclarationTransformer::createDCFD;
    using cAddapterCppFunctionDeclarationTransformer::composeDCFD;
    using cAddapterCppFunctionDeclarationTransformer::composeDerivedClassFunctionBody;

    using cAddapterCppFunctionDeclarationTransformer::derived;
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
    EXPECT_EQ(r, "virtual int foo(const std::string& s,int * ptr,double d)const = 0");
  }
}

TEST_F(test_cCppFunctionDeclarationParser, test_split0 )
{
  {
    Test_cCppFunctionDeclarationParser t;
    t.split0( "virtual int foo(const std::string& s, int * ptr, double d)const = 0" );

    EXPECT_EQ("virtual int", t.r.sReturn);
    EXPECT_EQ("foo", t.r.sName);
    EXPECT_EQ("const std::string& s, int * ptr, double d", t.r.sParameters);
    EXPECT_EQ("const = 0", t.r.sTailAttributes);
  }
}

TEST_F(test_cCppFunctionDeclarationParser, test_splitParameters )
{
  {
    Test_cCppFunctionDeclarationParser t;

    t.r.sParameters = "const std::string & s,int* ptr,double d";
    t.splitParameters();

    EXPECT_EQ("const std::string &", t.r.parameters[0].first );
    EXPECT_EQ("s", t.r.parameters[0].second );

    EXPECT_EQ("int*", t.r.parameters[1].first);
    EXPECT_EQ("ptr", t.r.parameters[1].second);

    EXPECT_EQ("double", t.r.parameters[2].first);
    EXPECT_EQ("d", t.r.parameters[2].second);
  }

  {
    Test_cCppFunctionDeclarationParser t;

    t.r.sParameters = "";
    t.splitParameters();

    EXPECT_EQ(0, t.r.parameters.size());

    t.r.sParameters = " ";
    t.splitParameters();

    EXPECT_EQ(0, t.r.parameters.size());
  }

  {
      Test_cCppFunctionDeclarationParser t;

      t.r.sParameters = "const std::string & s";
      t.splitParameters();

      EXPECT_EQ("const std::string &", t.r.parameters[0].first);
      EXPECT_EQ("s", t.r.parameters[0].second);
  }
}

TEST_F(test_cAddapterCppFunctionDeclarationTransformer, test_createDCFDReturn)
{
    {
        Test_cAddapterCppFunctionDeclarationTransformer t;

        t.derived.sReturn = " virtual     const std::string &";
        std::string res = t.createDCFDReturn();

        EXPECT_EQ(" const std::string &", res );
    }
}

TEST_F(test_cAddapterCppFunctionDeclarationTransformer, test_createDCFDTailAttributes)
{
    {
        Test_cAddapterCppFunctionDeclarationTransformer t;

        t.derived.sTailAttributes = " const  =    0 ;";
        std::string res = t.createDCFDTailAttributes();

        EXPECT_EQ(" const", res);
    }

    {
        Test_cAddapterCppFunctionDeclarationTransformer t;

        t.derived.sTailAttributes = " const=0;";
        std::string res = t.createDCFDTailAttributes();

        EXPECT_EQ(" const", res);
    }
}
TEST_F(test_cAddapterCppFunctionDeclarationTransformer, test_createcreateDCFD)
{
    {
        test_cCppFunctionDeclarationParser::Test_cCppFunctionDeclarationParser t0;
        Test_cAddapterCppFunctionDeclarationTransformer t;
        std::string decl = "virtual int foo(const std::string& s,  int * ptr, double d)const=0;";

        t0.split0(decl);
        t0.splitParameters();

        t.createDCFD(t0.ParseResult());

        EXPECT_EQ( t.derived.sReturn, "int");
        EXPECT_EQ( t.derived.sName, t.derived.sName);
        EXPECT_EQ( t.derived.parameters, t.derived.parameters);
        EXPECT_EQ(t.derived.sTailAttributes, "const");
    }
}

TEST_F(test_cAddapterCppFunctionDeclarationTransformer, test_composeDCFD)
{
    {
        test_cCppFunctionDeclarationParser::Test_cCppFunctionDeclarationParser t0;
        Test_cAddapterCppFunctionDeclarationTransformer t;
        std::string decl = "virtual int foo(const std::string& s,int* ptr,double d)const=0;";

        t0.split0(decl);
        t0.splitParameters();
        t.createDCFD(t0.ParseResult());

        std::string res = t.composeDCFD();

        EXPECT_EQ("int foo(const std::string& s,int* ptr,double d)const override", res);
    }
}

TEST_F(test_cAddapterCppFunctionDeclarationTransformer, test_composeDerivedClassFunctionBody)
{
    {
        test_cCppFunctionDeclarationParser::Test_cCppFunctionDeclarationParser t0;
        Test_cAddapterCppFunctionDeclarationTransformer t;
        t0.setClassName("TestClass");
        std::string decl = "virtual int Int()const=0;";

        t0.split0(decl);
        t0.splitParameters();
        t.setClassName("TestClass");
        t.createDCFD(t0.ParseResult());

        //std::string res0 = t.composeDerivedClassFunctionBody();
        //EXPECT_EQ("int foo(const std::string& s,int* ptr,double d)const override", res0);

        std::string resT = R""""({
    return IoC.Resolve<int>("TestClass.Int",obj);
}
)"""";
        std::string res0 = t.composeDerivedClassFunctionBody();

        EXPECT_EQ(resT, res0);
    }

    {
        test_cCppFunctionDeclarationParser::Test_cCppFunctionDeclarationParser t0;
        Test_cAddapterCppFunctionDeclarationTransformer t;
        t0.setClassName("TestClass");

        std::string decl = "virtual T& setT(const std::string& s,int* ptr,double d) = 0;";

        t0.split0(decl);
        t0.splitParameters();
        t.setClassName("TestClass");
        t.createDCFD(t0.ParseResult());

        std::string resT = R""""({
    IoC.Resolve<iCommand>("TestClass.setT",obj,s,ptr,d).Execute();
}
)"""";

        std::string res0 = t.composeDerivedClassFunctionBody();

        EXPECT_EQ(resT, res0);
    }
}
