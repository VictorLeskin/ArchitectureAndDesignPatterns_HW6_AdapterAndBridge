///************************* ITELMA SP ****************************************

#include <gtest/gtest.h>

#include "cgenerateadapter.hpp"
#include "ispaceship.hpp"


// clang-format off

// gTest grouping class
class test_cGenerateAdapter : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_cGenerateAdapter : public cGenerateAdapter
  {
  public:
    // add here members for free access.
    using cGenerateAdapter::cGenerateAdapter; // delegate constructors
  };

};
 
TEST_F(test_cGenerateAdapter, test_ctor )
{
  //Test_cGenerateAdapter t;
}


// gTest grouping class
class test_cInterfaceFileReader : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_cInterfaceFileReader : public cInterfaceFileReader
  {
  public:
    std::istringstream tmp_strm;
  public:
    // add here members for free access.
    Test_cInterfaceFileReader() : cInterfaceFileReader(tmp_strm) {}
    using cInterfaceFileReader::cInterfaceFileReader; // delegate constructors

    using cInterfaceFileReader::isAdapteeClassDeclaration;
    using cInterfaceFileReader::isVirtualFunctionDefinition;
    using cInterfaceFileReader::isClosingClassDefinition;
    using cInterfaceFileReader::startClass;
    using cInterfaceFileReader::addFunctionDeclaration;
    using cInterfaceFileReader::finishClass;

    using cInterfaceFileReader::interfaceClassesIdx;
    using cInterfaceFileReader::interfaceClasses;

    using cInterfaceFileReader::className;
    using cInterfaceFileReader::virtualFunctions;
  };

};

TEST_F(test_cInterfaceFileReader, test_isAdapteeClassDeclaration)
{
  Test_cInterfaceFileReader t;

  EXPECT_TRUE(t.isAdapteeClassDeclaration(std::string("class /*ADAPTED*/ cAdaptee")));
  EXPECT_TRUE(t.isAdapteeClassDeclaration(std::string("\tclass  /*ADAPTED*/     cAdaptee  \t")));
  EXPECT_TRUE(t.isAdapteeClassDeclaration(std::string("class /*ADAPTED*/ className")));
  EXPECT_TRUE(t.isAdapteeClassDeclaration(std::string("   class    /*ADAPTED*/ \t cAdaptee  ")));
  EXPECT_FALSE(t.isAdapteeClassDeclaration(std::string("   class    /*ADAPTED */ \t cAdaptee  ")));
  EXPECT_FALSE(t.isAdapteeClassDeclaration(std::string("    struct /*ADAPTED*/ \t cAdaptee ")));
  EXPECT_FALSE(t.isAdapteeClassDeclaration(std::string("not a class /*ADAPTED*/ \t cAdaptee  ")));

}


TEST_F(test_cInterfaceFileReader, test_isVirtualFunctionDefinition)
{
  Test_cInterfaceFileReader t;
  
  EXPECT_TRUE(t.isVirtualFunctionDefinition(std::string("virtual int func()=0;")));
  EXPECT_TRUE(t.isVirtualFunctionDefinition(std::string("virtual int func() const =0;")));
  EXPECT_TRUE(t.isVirtualFunctionDefinition(std::string("   \tvirtual   int   func()  =  0  ;  ")));
  EXPECT_TRUE(t.isVirtualFunctionDefinition(std::string("   \tvirtual   int   func( double x, const cClass &cl )  =  0  ;  ")));
  EXPECT_FALSE(t.isVirtualFunctionDefinition(std::string("int   func() ; ")));
  EXPECT_FALSE(t.isVirtualFunctionDefinition(std::string("//virtual int func() const =0;")));
}

TEST_F(test_cInterfaceFileReader, test_isClosingClassDefinition)
{
  Test_cInterfaceFileReader t;

  EXPECT_TRUE(t.isClosingClassDefinition(std::string("};")));
  EXPECT_TRUE(t.isClosingClassDefinition(std::string("  \t} ;      ")));
  EXPECT_FALSE(t.isClosingClassDefinition(std::string("int a;")));
  EXPECT_FALSE(t.isClosingClassDefinition(std::string("}")));
  EXPECT_FALSE(t.isClosingClassDefinition(std::string(";}")));
}


TEST_F(test_cInterfaceFileReader, test_startClass)
{
  {
    Test_cInterfaceFileReader t;
    t.virtualFunctions.push_back("");

    t.startClass(std::string("class /*ADAPTED*/ cAdaptee"));

    EXPECT_EQ("cAdaptee", t.className);
    EXPECT_EQ(std::vector<std::string>(), t.virtualFunctions);
  }

  {
    try
    {
      Test_cInterfaceFileReader t;
      t.virtualFunctions.push_back("");

      t.startClass(std::string("class /*ADAPTED*/ "));
      FAIL();
    }
    catch (const std::exception& expected)
    {
      ASSERT_STREQ("Can't get the class name from a class name desription", expected.what());
    }
  }

}

TEST_F(test_cInterfaceFileReader, test_addFunctionDeclaration)
{
  Test_cInterfaceFileReader t;

  t.addFunctionDeclaration("virtual int func()=0;");

  ASSERT_EQ(1, t.virtualFunctions.size());
  ASSERT_EQ(std::string("virtual int func()=0;"), t.virtualFunctions[0]);
}

TEST_F(test_cInterfaceFileReader, test_finishClass)
{
  Test_cInterfaceFileReader t;

  t.startClass(std::string("class /*ADAPTED*/ cAdaptee"));
  t.addFunctionDeclaration("virtual int func()=0;");
  t.addFunctionDeclaration("virtual int func1( double x, const cClass &cl )=0;");

  t.finishClass("};");

  ASSERT_EQ(1, t.interfaceClasses.size());
  ASSERT_EQ("cAdaptee", t.interfaceClasses[0].ClassName());
  ASSERT_EQ(t.interfaceClasses[0].Function(0).sFunctionDeclaration, "virtual int func()=0;");
  ASSERT_EQ(t.interfaceClasses[0].Function(1).sFunctionDeclaration, "virtual int func1( double x, const cClass &cl )=0;" );
}

