///************************* ITELMA SP ****************************************
#ifndef CGENERATEADAPTER_HPP
#define CGENERATEADAPTER_HPP

#include <tuple>
#include <string>

#include "ccppfunctiondeclarationparser.hpp"

// This class generates adapters from special formatted interface classes.
// The main function of the class opens a file, looks a interface classes and 
// generates adapter classes for found interfaces.
class cGenerateAdapter
{
public:
  // return 0  if successful otherwise nonzer and a message
  static std::tuple<int, std::string> main(int argc, const char* argv[]);
};

class cInterfaceClass
{
public:
  std::string ClassName() const { return className; }

protected:
  std::string className;
  std::vector<sParserResult> functions;
};

class cAdapterClass
{
public:
  std::string ClassName() const { return interfaceClass->ClassName();  }

protected:
  cInterfaceClass* interfaceClass;
};

class cInterfaceFileReader
{
private:
  cInterfaceFileReader(const cInterfaceFileReader&);
  cInterfaceFileReader &operator=(const cInterfaceFileReader&);

public:
  cInterfaceFileReader(std::istream& s) : strm(s) {}

  void read();

  bool empty() const { return interfaceClasses.empty();  }
  const cInterfaceClass* getClass()
  {
    return (interfaceClassesIdx < interfaceClasses.size())
      ? &interfaceClasses[interfaceClassesIdx++] 
      : nullptr;
  }

protected:
  std::istream& strm;
  int interfaceClassesIdx = 0;
  std::vector<cInterfaceClass> interfaceClasses;
};

class cAdapterClassesGenerator 
{
public:
  cAdapterClass create(const cInterfaceClass&);
};

class cAdapterClassesSourceFile
{
public:
  cAdapterClassesSourceFile(int argc, const char* argv[]);

  void write(const cAdapterClass &);

protected:
  void open();
  void writeHeader();
};



#endif //#ifndef CGENERATEADAPTER_HPP
