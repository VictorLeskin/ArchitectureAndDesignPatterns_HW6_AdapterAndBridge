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
  static std::tuple<int, std::string> main(std::string inputFileName, std::istream& istrm, std::string outputFileName, std::ostream& ostrm);

  static std::string OutputFileName(std::string inputFileName) { return std::string("adapter_") + inputFileName; }
};

class cInterfaceClass
{
public:
  const std::string &ClassName() const { return className; }
  const sParserResult& Function(int idx) const { return functions[idx]; }
  int FunctionCount() const { return int(functions.size()); }
  const std::vector<sParserResult> &Functions() const { return functions; }

  cInterfaceClass() {}
  cInterfaceClass(const std::string& className, const std::vector<std::string>& virtualFunctionsDeclarations);

protected:
  std::string className;
  std::vector<sParserResult> functions;
};

class cAdapterClass
{
public:
  cAdapterClass(const std::string interfaceClassName, const std::string className,
    const std::vector<sDerivedClassFunction> &functions ) : interfaceClassName(interfaceClassName), className(className), functions(functions)
  { }

  std::string ToStr() const;
    

protected:
    std::string interfaceClassName;
    std::string className;
    std::vector<sDerivedClassFunction> functions;
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
  bool isAdapteeClassDeclaration( const std::string &s ) const;
  bool isVirtualFunctionDefinition( const std::string &s ) const;
  bool isClosingClassDefinition(const std::string& s) const;

  void startClass(const std::string &s );
  void addFunctionDeclaration(const std::string &s );
  void finishClass(const std::string& s);

protected:
  std::istream& strm;
  int interfaceClassesIdx = 0;
  std::vector<cInterfaceClass> interfaceClasses;

  std::string className;
  std::vector<std::string> virtualFunctions;
};

class cAdapterClassesSourceFile
{
public:
  cAdapterClassesSourceFile(std::ostream& s) : strm(s) 
  {
    writeHeader();
  }

  void write(const cAdapterClass &);

protected:
  void writeHeader();

protected:
  std::ostream& strm;
};



#endif //#ifndef CGENERATEADAPTER_HPP
