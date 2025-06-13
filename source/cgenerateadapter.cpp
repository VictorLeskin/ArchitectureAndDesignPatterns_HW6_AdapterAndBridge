///************************* OUTS HOMEWORK ****************************************
#include "cgenerateadapter.hpp"
#include <fstream>
#include <sstream>
#include <regex>

static auto replaceAll = [](std::string& str, const std::string& from, const std::string& to) 
{
  if (from.empty())
    return;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) 
  {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // Move past the replaced part to avoid infinite loops
  }
};


std::tuple<int, std::string> cGenerateAdapter::main(std::string inputFileName, std::istream& istrm, std::string outputFileName, std::ostream& ostrm)
{
    std::string report;

    cInterfaceFileReader ifr(istrm);
    cAddapterCppFunctionDeclarationTransformer acg;
    cAdapterClassesSourceFile acsf(inputFileName, outputFileName, ostrm);
    

    try
    {
        ifr.read();

        if (false == ifr.empty())
        {
            acsf.writeHeader();
            while (auto iClass = ifr.getClass())
            {
                auto ac = acg.createAdapterClass(iClass);
                acsf.write(*ac);

                // init report string or add comma to separate class names
                report = (report == "") ? std::string("Generated adapter for classes: ") : (report + ", ");
                report += iClass->ClassName();
            }
            acsf.finishFile();
        }
        else
            return std::tuple(2, "No interface class found.");
    }
    catch (const std::exception& expected)
    {
        return std::tuple(3, std::string("Something is wrong: ") + expected.what());
    }

    return std::tuple(0, "");
}


std::tuple<int, std::string> cGenerateAdapter::main(int argc, const char* argv[])
{
  if (argc != 2)
    return std::tuple(1, "Missed interface header file\nUsage:[interface header file].");

  std::ifstream istrm(argv[1]);
  if (true == istrm.fail())
      return std::tuple(1, std::string("Can't open input file") + argv[1]);

  std::string outputFileName = OutputFileName(argv[1]);
  std::ofstream ostrm(outputFileName);

  return main(argv[1], istrm, outputFileName, ostrm);
}

void cInterfaceFileReader::read()
{
  enum enumState { INIT, COLLECT_VIRTUAL_FUNCTIONS } eState(INIT);
  std::string s;
  while( std::getline(strm,s) )
  { 
    switch (eState)
    {
    case INIT:
      if (isAdapteeClassDeclaration(s))
      {
        startClass(s);
        eState = COLLECT_VIRTUAL_FUNCTIONS;
      }
    case COLLECT_VIRTUAL_FUNCTIONS:
      if (isVirtualFunctionDefinition(s))
        addFunctionDeclaration(s);
      else if( isClosingClassDefinition(s))
      {
        eState = INIT;
        finishClass(s);
      }
    }
  }
}

void cAdapterClassesSourceFile::write(const cAdapterClass& a)
{
  strm <<  a.ToStr();
}

void cAdapterClassesSourceFile::writeHeader() 
{
    const std::string t = R""""(///************************* OUTS HOMEWORK ****************************************

// Automatically generated adapter classes.
#ifndef $GUARD_DEFINE$
#define $GUARD_DEFINE$

#include "$BASE_CLASSES_FILE$"
#include "iAdapterObj.hpp"
#include "iCommand.hpp"
#include "cIoC.hpp"
)"""";

    std::string ret(t);
    std::string guard = outputFileName;
    std::transform(guard.begin(), guard.end(), guard.begin(), ::toupper);
    replaceAll(guard, ".HPP", "_HPP" );
    replaceAll(ret, "$GUARD_DEFINE$", guard);
    replaceAll(ret, "$BASE_CLASSES_FILE$", inputFileName);
    strm << ret;
}

void cAdapterClassesSourceFile::finishFile()
{
    const std::string t = R""""(
#endif //#ifndef $GUARD_DEFINE$
)"""";

    std::string ret(t);
    std::string guard = outputFileName;
    std::transform(guard.begin(), guard.end(), guard.begin(), ::toupper);
    replaceAll(ret, "$GUARD_DEFINE$", guard);
    strm << ret;
}


bool cInterfaceFileReader::isAdapteeClassDeclaration(const std::string& s) const 
{ 
  // class /*ADAPTED*/ className
  std::regex r(R"(\s*class\s+/\*ADAPTED\*/\s+\w+\s*)");
  return std::regex_match(s, r);
}

bool cInterfaceFileReader::isVirtualFunctionDefinition(const std::string& s) const 
{ 
  // should be pure virutal function
  // virtual return values funcName() = 0;
  // check "virtual " and "=0;" 
  std::regex r(R"(\s*virtual\s+[^=]+=\s*0\s*;\s*)");
  return std::regex_match(s, r);
}
bool cInterfaceFileReader::isClosingClassDefinition(const std::string& s) const 
{ 
  // '};'
  std::regex r(R"(\s*\}\s*;\s*)");
  return std::regex_match(s, r);
}

void cInterfaceFileReader::startClass(const std::string& s) 
{ 
  // gen name
  std::regex r(R"(\s*class\s+/\*ADAPTED\*/\s+(\w+)\s*)");
  std::smatch m;
  std::regex_match(s, m, r);

  if (m.size() != 2)
    throw(std::exception("Can't get the class name from a class name desription"));

  className = m[1];
  virtualFunctions.clear();
}

void cInterfaceFileReader::addFunctionDeclaration(const std::string& s) 
{ 
  virtualFunctions.push_back(s);
}

void cInterfaceFileReader::finishClass( [[maybe_unused]] const std::string& s)
{
  cInterfaceClass ic( className, virtualFunctions);
  interfaceClasses.push_back(ic);
}

cInterfaceClass::cInterfaceClass(const std::string& className, const std::vector<std::string>& virtualFunctionsDeclarations):
  className(className)
{
  cCppFunctionDeclarationParser parser;
  for( const auto &sFunc:virtualFunctionsDeclarations )
  {
    sParserResult r = parser.parse(sFunc);
    functions.push_back(r);
  }
}


std::string cAdapterClass::ToStr() const
{ 
    const std::string t = R""""(
// Automatically generated adapter class.
class $CLASS_NAME$ : protected iAdapterObj, public $INTERFACE_CLASS_NAME$
{
public:  	
  $CLASS_NAME$() : obj(this)
  {
  }

$FUNCTIONS$protected:
  iAdapterObj *obj;  	
};
)"""";

    std::string ret(t);
    replaceAll(ret, "$CLASS_NAME$", className);
    replaceAll(ret, "$INTERFACE_CLASS_NAME$", interfaceClassName);

    std::ostringstream strm;
    for( const auto f: functions )
    {
        strm << "  " << f.sFunctionDeclaration << std::endl;
        strm << "  " << "{" << std::endl;
        strm << "  " << "  " << f.tBody << std::endl;
        strm << "  " << "}" << std::endl << std::endl;
    }

    replaceAll(ret, "$FUNCTIONS$", strm.str() );

    return ret;
}
