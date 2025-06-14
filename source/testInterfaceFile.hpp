///************************* OUTS HOMEWORK ****************************************
#ifndef TESTINTERFACEFILE_HPP 
#define TESTINTERFACEFILE_HPP 

#include <tuple>
#include <string>

#include "ccppfunctiondeclarationparser.hpp"

// This class generates adapters from special formatted interface classes.
// The main function of the class opens a file, looks a interface classes and 
// generates adapter classes for found interfaces.
// if class definition contains /*ADAPTED*/ between 'class' and class name this class is adaptee
// fuction starting with uppercase return part of state of class
// function starting with lowercase are actor and will be adapted by execution of a command so they return 'void'
class /*ADAPTED*/ testGenerateAdapter
{
public:
  // return 0  if successful otherwise nonzer and a message
  virtual std::tuple<int, std::string> main(int argc, const char** argv) const = 0;
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

  virtual  bool IsEmpty() const = 0;
  virtual  const testInterfaceClass* Class() const = 0;

protected:
  virtual bool IsAdapteeClassDeclaration(const std::string& s) const = 0;
  virtual bool IsVirtualFunctionDefinition(const std::string& s) const = 0;
  virtual bool IsClosingClassDefinition(const std::string& s) const = 0;

  virtual void startClass(const std::string& s) = 0;
  virtual void addFunctionDeclaration(const std::string& s) = 0;
  virtual void finishClass(const std::string& s) = 0;
};

class /*ADAPTED*/ testAdapterClassesGenerator
{
public:
  virtual void createAdapterClass(const testInterfaceClass& s) = 0;
  virtual testAdapterClass GetAdapterClass() const;

};

class /*ADAPTED*/ testAdapterClassesSourceFile
{
public:
  virtual void write(const testAdapterClass& s) = 0;

protected:
  virtual void open() = 0;
  virtual void writeHeader() = 0;
};

#endif //#ifndef TESTINTERFACEFILE_HPP 
