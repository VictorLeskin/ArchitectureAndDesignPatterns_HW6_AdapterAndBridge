///************************* OUTS HOMEWORK ****************************************
#ifndef ISPACESHIP_HPP
#define ISPACESHIP_HPP

#include "cvector.hpp"

//The input file with interface classes shold follow a special format.
//Main purpose of the format is easy generation a adapter of the class.
//The format is as follows:
// no double spaces are allowed in function definition except the start of a line: the penalty is a exception and no generated adapter.
// class definition should include  /*ADAPTED*/  attribute between 'class' and class name like below
// class  /*ADAPTED*/  iSpaceship
// each virtual function starting by 'virtual' and will be overrided in the derived class.
// Uppercase letter in name marks getters and lowercases for setters

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

#endif //#ifndef ISPACESHIP_HPP
