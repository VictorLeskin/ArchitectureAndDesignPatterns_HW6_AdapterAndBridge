///************************* OUTS HOMEWORK ****************************************
#ifndef ISPACESHIP_HPP
#define ISPACESHIP_HPP

#include "cvector.hpp"

// This interface class declaraion have a special format.
// Main purpose of the format is easy generation a adapter of the class.
// The format is as follows:
// no double spaces are allowed in gettexcept the start of a line: the penalty is a exception and no generated adapter.
// class definition should include  /*autoadapted*/  attribute between class and class name like below
// class  /*ADAPTED*/  iSpaceship
// each virtual function starting by get or set followed uppercase letter will be replaced by 
// call of 'adapted' function

class  /*ADAPTED*/  iSpaceship
{
public:
  virtual ~iSpaceship() = default;

  // access
  virtual const cVector& getPosition() const = 0;
  virtual const cVector& getVelocity() const = 0;
  virtual std::string getName() const = 0;

  // change
  virtual void setPosition(const cVector& pos) = 0;
  virtual void setVelocity(const cVector& vel) = 0;
  virtual void setVelocity(double angle, double velocity ) = 0;
};

#endif //#ifndef ISPACESHIP_HPP
