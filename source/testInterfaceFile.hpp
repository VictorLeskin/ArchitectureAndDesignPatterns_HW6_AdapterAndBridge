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
