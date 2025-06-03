///************************* ITELMA SP ****************************************

#include "cgenerateadapter.hpp"
#include <fstream>

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
          auto ic = acg.create(*iClass);
          acsf.write(ic);

          // init report string or add comma to separate class names
          report = (report == "") ? std::string("Generated adapter for classes: ") : (report + ", ");
          report += ic.ClassName();
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
  throw std::exception("not imiplemented");
}

cAdapterClass cAdapterClassesGenerator::create(const cInterfaceClass&)
{
  throw std::exception("not imiplemented");
  return cAdapterClass();
}

cAdapterClassesSourceFile::cAdapterClassesSourceFile(int argc, const char* argv[])
{
  throw std::exception("not imiplemented");
}

void cAdapterClassesSourceFile::write(const cAdapterClass&)
{
  throw std::exception("not imiplemented");
}
