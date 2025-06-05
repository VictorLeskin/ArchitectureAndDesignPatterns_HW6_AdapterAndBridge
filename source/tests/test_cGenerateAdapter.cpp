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
    Test_cInterfaceFileReader(std::istringstream &strm) : cInterfaceFileReader(strm) {}

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

    static std::string testInterfaceFile_hpp;
  };

};


// gTest grouping class
class test_cAdapterClassesGenerator : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_cAdapterClassesGenerator : public cAdapterClassesGenerator
  {
  public:
    // add here members for free access.
    using cAdapterClassesGenerator::cAdapterClassesGenerator; // delegate constructors
  };

};

TEST_F(test_cAdapterClassesGenerator, test_create)
{
  std::string t0("TestClass");
  std::vector<std::string> t1({
  "virtual void write(const testAdapterClass&) = 0;",
  "virtual void open() = 0;",
  "virtual void writeHeader() = 0;" });
  cInterfaceClass p0(t0, t1);

  Test_cAdapterClassesGenerator t;
  cAdapterClass r = t.create( p0 );
}

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

TEST_F(test_cInterfaceFileReader, test_read)
{
  std::istringstream strm(Test_cInterfaceFileReader::testInterfaceFile_hpp);
  Test_cInterfaceFileReader t(strm);

  t.read();

  ASSERT_EQ(6, t.interfaceClasses.size());
  EXPECT_EQ("testGenerateAdapter", t.interfaceClasses[0].ClassName());
  EXPECT_EQ("testInterfaceClass", t.interfaceClasses[1].ClassName());
  EXPECT_EQ("testAdapterClass", t.interfaceClasses[2].ClassName());
  EXPECT_EQ("testInterfaceFileReader", t.interfaceClasses[3].ClassName());
  EXPECT_EQ("testAdapterClassesGenerator", t.interfaceClasses[4].ClassName());
  EXPECT_EQ("testAdapterClassesSourceFile", t.interfaceClasses[5].ClassName());

  ASSERT_EQ(1, t.interfaceClasses[0].FunctionCount() );
  ASSERT_EQ(2, t.interfaceClasses[1].FunctionCount());
  ASSERT_EQ(1, t.interfaceClasses[2].FunctionCount());
  ASSERT_EQ(9, t.interfaceClasses[3].FunctionCount());
  ASSERT_EQ(1, t.interfaceClasses[4].FunctionCount());
  ASSERT_EQ(3, t.interfaceClasses[5].FunctionCount());
}

TEST_F(test_cInterfaceFileReader, test_empty)
{
  Test_cInterfaceFileReader t;
  EXPECT_EQ(true, t.empty());
  t.interfaceClasses.push_back(cInterfaceClass("className", std::vector<std::string>({"virtual void write(const testAdapterClass&) = 0"})));
  EXPECT_EQ(false, t.empty());
}

TEST_F(test_cInterfaceFileReader, test_getClass)
{
  {
    Test_cInterfaceFileReader t;
    EXPECT_EQ(nullptr, t.getClass());
  }

  {
    Test_cInterfaceFileReader t;
    t.interfaceClasses.push_back(cInterfaceClass("className", std::vector<std::string>({ "virtual void write(const testAdapterClass&) = 0" })));
    EXPECT_EQ( &t.interfaceClasses[0], t.getClass());
    EXPECT_EQ(nullptr, t.getClass());
  }
}


std::string test_cInterfaceFileReader::Test_cInterfaceFileReader::testInterfaceFile_hpp(R""""(
#ifndef CGENERATEADAPTER_HPP
#define CGENERATEADAPTER_HPP

#include <tuple>
#include <string>

#include "ccppfunctiondeclarationparser.hpp"

// This class generates adapters from special formatted interface classes.
// The main function of the class opens a file, looks a interface classes and 
// generates adapter classes for found interfaces.
class /*ADAPTED*/ testGenerateAdapter
{
public:
  // return 0  if successful otherwise nonzer and a message
  virtual std::tuple<int, std::string> main(int argc, const char* argv[]) const = 0;
};

class /*ADAPTED*/ testInterfaceClass
{
public:
  virtual const std::string& ClassName() const = 0;
  virtual const sParserResult& Function(int idx) const = 0;
};

class /*ADAPTED*/ testAdapterClass
{
public:
  virtual std::string ClassName() const = 0;
};

class /*ADAPTED*/ testInterfaceFileReader
{
  virtual  void read() = 0;

  virtual  bool empty() const = 0;
  virtual  const testInterfaceClass* getClass() const = 0;

protected:
  virtual bool isAdapteeClassDeclaration(const std::string& s) const = 0;
  virtual bool isVirtualFunctionDefinition(const std::string& s) const = 0;
  virtual bool isClosingClassDefinition(const std::string& s) const = 0;

  virtual void startClass(const std::string& s) = 0;
  virtual void addFunctionDeclaration(const std::string& s) = 0;
  virtual void finishClass(const std::string& s) = 0;
};

class /*ADAPTED*/ testAdapterClassesGenerator
{
public:
  virtual testAdapterClass create(const testInterfaceClass&)  const = 0;
};

class /*ADAPTED*/ testAdapterClassesSourceFile
{
public:
  virtual void write(const testAdapterClass&) = 0;

protected:
  virtual void open() = 0;
  virtual void writeHeader() = 0;
};

#endif //#ifndef CGENERATEADAPTER_HPP
)"""");










