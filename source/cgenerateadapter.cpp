///************************* ITELMA SP ****************************************

#include "cgenerateadapter.hpp"
#include <fstream>
#include <regex>

std::tuple<int, std::string> cGenerateAdapter::main(int argc, const char* argv[])
{
  if (argc != 2)
    return std::tuple(1, "Missed interface header file\nUsage:[interface header file].");
  
  std::ifstream strm(argv[1]);
  std::string report;

  if( false == strm.fail() )
  {
    cInterfaceFileReader ifr(strm);
    cAdapterClassesGenerator acg;
    cAdapterClassesSourceFile acsf(argc, argv);

    try
    {
      ifr.read();

      if (false == ifr.empty())
      {
        while (auto iClass = ifr.getClass())
        {
          auto ac = acg.create(*iClass);
          acsf.write(ac);

          // init report string or add comma to separate class names
          report = (report == "") ? std::string("Generated adapter for classes: ") : (report + ", ");
          report += ac.ClassName();
        }
      }
      else
        return std::tuple(2, "No interface class found.");
    }
    catch (const std::exception& expected)
    {
      return std::tuple(3, std::string( "Something is wrong: ") + expected.what());
    }
  }
  else 
    return std::tuple(1, std::string( "Can't open input file") + argv[1] );
    
  return std::tuple(0,"");
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

cAdapterClass cAdapterClassesGenerator::create(const cInterfaceClass& ic)
{
  return cAdapterClass(ic);
}

cAdapterClassesSourceFile::cAdapterClassesSourceFile(int argc, const char* argv[])
{
  throw std::exception("not imiplemented");
}

void cAdapterClassesSourceFile::write(const cAdapterClass&)
{
  throw std::exception("not imiplemented");
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

inline cAdapterClass::cAdapterClass(const cInterfaceClass& ic) : interfaceClass(&ic)
{
  create();
}

void cAdapterClass::create()
{
}
