///************************* ITELMA SP ****************************************

#include <gtest/gtest.h>

#include "cgenerateadapter.hpp"
#include "ispaceship.hpp"


// clang-format off

// gTest grouping class
class test_cGenerateAdapter : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_cGenerateAdapter : public cGenerateAdapter
  {
  public:
    // add here members for free access.
    using cGenerateAdapter::cGenerateAdapter; // delegate constructors
  };

};
 
TEST_F(test_cGenerateAdapter, test_ctor )
{
  //Test_cGenerateAdapter t;
}
