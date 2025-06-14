///************************* OUTS HOMEWORK ****************************************

// Automatically generated adapter classes.
#ifndef ADAPTER_ISPACESHIP_HPP
#define ADAPTER_ISPACESHIP_HPP

#include "ispaceship.hpp"
#include "iAdapterObj.hpp"
#include "iCommand.hpp"
#include "cIoC.hpp"

// Automatically generated adapter class.
class cAdapter_iSpaceship : protected iAdapterObj, public iSpaceship
{
public:  	
  cAdapter_iSpaceship() : obj(this)
  {
  }

  const cVector& getPosition()const override
  {
    IoC.Resolve<iCommand *>("cAdapter_iSpaceship.getPosition",obj)->Execute();
  }

  const cVector& getVelocity()const override
  {
    IoC.Resolve<iCommand *>("cAdapter_iSpaceship.getVelocity",obj)->Execute();
  }

  std::string getName()const override
  {
    IoC.Resolve<iCommand *>("cAdapter_iSpaceship.getName",obj)->Execute();
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

#endif //#ifndef ADAPTER_ISPACESHIP.HPP
