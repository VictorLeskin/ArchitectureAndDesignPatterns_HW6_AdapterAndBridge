///************************* OUTS HOMEWORK ****************************************
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

  static std::string OutputFileName(std::string inputFileName) { return std::string("Adapter_") + inputFileName; }
};


// input iterface class.
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

// output iterface class.
class cAdapterClass
{
public:
  cAdapterClass(const std::string interfaceClassName, const std::string className,
    const std::vector<sDerivedClassFunction> &functions ) : interfaceClassName(interfaceClassName), className(className), functions(functions)
  { }

  std::string ToStr() const; // generate class definition 

protected:
    std::string interfaceClassName;
    std::string className;
    std::vector<sDerivedClassFunction> functions;
};


// intput file reader. Read file line by line and generate a list of 
// interface classes in special format.
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

  // current clas name and functions
  std::string className;
  std::vector<std::string> virtualFunctions;
};

// output header file writer. Write header and each class. Finish it by adding closing preprocessor directive
class cAdapterFileWriter
{
public:
  cAdapterFileWriter(std::string inputFileName, std::string outputFileName, std::ostream& s) :
      inputFileName(inputFileName),
      outputFileName(outputFileName),
      strm(s)
  {
  }

  void writeHeader();
  void write(const cAdapterClass &);
  void finishFile();

  std::string makeGuard() const;

protected:
  std::string inputFileName;
  std::string outputFileName;
  std::ostream& strm;
};



#endif //#ifndef CGENERATEADAPTER_HPP
