///************************* OUTS HOMEWORK ****************************************

// Automatically generated adapter classes.
#ifndef ADAPTER_TESTINTERFACEFILE_HPP
#define ADAPTER_TESTINTERFACEFILE_HPP

#include "testInterfaceFile.hpp"
#include "iAdapterObj.hpp"
#include "iCommand.hpp"
#include "cIoC.hpp"

// Automatically generated adapter class.
class cAdapter_testGenerateAdapter : protected iAdapterObj, public testGenerateAdapter
{
public:  	
  cAdapter_testGenerateAdapter() : obj(this)
  {
  }

  std::tuple<int,std::string> main(int argc,const char** argv)const override
  {
    IoC.Resolve<iCommand *>("cAdapter_testGenerateAdapter.main",obj,argc,argv)->Execute();
  }

protected:
  iAdapterObj *obj;  	
};

// Automatically generated adapter class.
class cAdapter_testInterfaceClass : protected iAdapterObj, public testInterfaceClass
{
public:  	
  cAdapter_testInterfaceClass() : obj(this)
  {
  }

  const std::string& ClassName()const override
  {
    return IoC.Resolve<const std::string&>("cAdapter_testInterfaceClass.ClassName",obj);
  }

  const sParserResult& Function(int idx)const override
  {
    return IoC.Resolve<const sParserResult&>("cAdapter_testInterfaceClass.Function",obj,idx);
  }

protected:
  iAdapterObj *obj;  	
};

// Automatically generated adapter class.
class cAdapter_testAdapterClass : protected iAdapterObj, public testAdapterClass
{
public:  	
  cAdapter_testAdapterClass() : obj(this)
  {
  }

  std::string ClassName()const override
  {
    return IoC.Resolve<std::string>("cAdapter_testAdapterClass.ClassName",obj);
  }

protected:
  iAdapterObj *obj;  	
};

// Automatically generated adapter class.
class cAdapter_testInterfaceFileReader : protected iAdapterObj, public testInterfaceFileReader
{
public:  	
  cAdapter_testInterfaceFileReader() : obj(this)
  {
  }

  void read() override
  {
    IoC.Resolve<iCommand *>("cAdapter_testInterfaceFileReader.read",obj)->Execute();
  }

  bool IsEmpty()const override
  {
    return IoC.Resolve<bool>("cAdapter_testInterfaceFileReader.IsEmpty",obj);
  }

  const testInterfaceClass* Class()const override
  {
    return IoC.Resolve<const testInterfaceClass*>("cAdapter_testInterfaceFileReader.Class",obj);
  }

  bool IsAdapteeClassDeclaration(const std::string& s)const override
  {
    return IoC.Resolve<bool>("cAdapter_testInterfaceFileReader.IsAdapteeClassDeclaration",obj,s);
  }

  bool IsVirtualFunctionDefinition(const std::string& s)const override
  {
    return IoC.Resolve<bool>("cAdapter_testInterfaceFileReader.IsVirtualFunctionDefinition",obj,s);
  }

  bool IsClosingClassDefinition(const std::string& s)const override
  {
    return IoC.Resolve<bool>("cAdapter_testInterfaceFileReader.IsClosingClassDefinition",obj,s);
  }

  void startClass(const std::string& s) override
  {
    IoC.Resolve<iCommand *>("cAdapter_testInterfaceFileReader.startClass",obj,s)->Execute();
  }

  void addFunctionDeclaration(const std::string& s) override
  {
    IoC.Resolve<iCommand *>("cAdapter_testInterfaceFileReader.addFunctionDeclaration",obj,s)->Execute();
  }

  void finishClass(const std::string& s) override
  {
    IoC.Resolve<iCommand *>("cAdapter_testInterfaceFileReader.finishClass",obj,s)->Execute();
  }

protected:
  iAdapterObj *obj;  	
};

// Automatically generated adapter class.
class cAdapter_testAdapterClassesGenerator : protected iAdapterObj, public testAdapterClassesGenerator
{
public:  	
  cAdapter_testAdapterClassesGenerator() : obj(this)
  {
  }

  void createAdapterClass(const testInterfaceClass& s) override
  {
    IoC.Resolve<iCommand *>("cAdapter_testAdapterClassesGenerator.createAdapterClass",obj,s)->Execute();
  }

protected:
  iAdapterObj *obj;  	
};

// Automatically generated adapter class.
class cAdapter_testAdapterClassesSourceFile : protected iAdapterObj, public testAdapterClassesSourceFile
{
public:  	
  cAdapter_testAdapterClassesSourceFile() : obj(this)
  {
  }

  void write(const testAdapterClass& s) override
  {
    IoC.Resolve<iCommand *>("cAdapter_testAdapterClassesSourceFile.write",obj,s)->Execute();
  }

  void open() override
  {
    IoC.Resolve<iCommand *>("cAdapter_testAdapterClassesSourceFile.open",obj)->Execute();
  }

  void writeHeader() override
  {
    IoC.Resolve<iCommand *>("cAdapter_testAdapterClassesSourceFile.writeHeader",obj)->Execute();
  }

protected:
  iAdapterObj *obj;  	
};

#endif //#ifndef ADAPTER_TESTINTERFACEFILE_HPP
