# ArchitectureAndDesignPatterns_HW6_AdapterAndBridge - homework #6 of OTUS course
The result of the homework  is a utility to generate adapter classes from interface classes.
The input file with interface classes shold follow a special format.
Main purpose of the format is easy generation a adapter of the class.
The format is as follows:
// no double spaces are allowed in function definition except the start of a line: the penalty is a exception and no generated adapter.
// class definition should include  /*ADAPTED*/  attribute between 'class' and class name like below
// class  /*ADAPTED*/  iSpaceship
// each virtual function starting by 'virtual' and will be overrided in the derived class.
// Uppercase letter in name marks getters and lowercases for setters

!!!Example of input interface file 
class  /*ADAPTED*/  iSpaceship
{
public:
  virtual ~iSpaceship() = default;

  // access
  virtual const cVector& Position() const = 0;
  virtual const cVector& Velocity() const = 0;
  virtual std::string Name() const = 0;

  // change
  virtual void setPosition(const cVector& pos) = 0;
  virtual void setVelocity(const cVector& vel) = 0;
  virtual void setVelocity(double angle, double velocity ) = 0;
};

!!!Result of processing above class.

// Automatically generated adapter class.
class cAdapter_iSpaceship : protected iAdapterObj, public iSpaceship
{
public:  	
  cAdapter_iSpaceship() : obj(this)
  {
  }

  const cVector& Position()const override
  {
    return IoC.Resolve<const cVector&>("cAdapter_iSpaceship.Position",obj);
  }

  const cVector& Velocity()const override
  {
    return IoC.Resolve<const cVector&>("cAdapter_iSpaceship.Velocity",obj);
  }

  std::string Name()const override
  {
    return IoC.Resolve<std::string>("cAdapter_iSpaceship.Name",obj);
  }

  void setPosition(const cVector& pos) override
  {
    IoC.Resolve<iCommand *>("cAdapter_iSpaceship.setPosition",obj,pos)->Execute();
  }

  void setVelocity(const cVector& vel) override
  {
    IoC.Resolve<iCommand *>("cAdapter_iSpaceship.setVelocity",obj,vel)->Execute();
  }

  void setVelocity(double angle,double velocity) override
  {
    IoC.Resolve<iCommand *>("cAdapter_iSpaceship.setVelocity",obj,angle,velocity)->Execute();
  }

protected:
  iAdapterObj *obj;  	
};

See also ispaceship.hpp testInterfaceFile.hpp Adapter_ispaceship.hpp Adapter_testInterfaceFile.hpp  files in source directory

Utility building:
cl /std:c++17 cgenerateadapter.cpp main_cgenerateadapter.cpp ccppfunctiondeclarationparser.cpp 
