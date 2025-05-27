///************************* ITELMA SP ****************************************

#include <gtest/gtest.h>

#include "cgenerateaadapter.hpp"
#include "ispaceship.hpp"


// clang-format off

// gTest grouping class
class test_cGenerateaAdapter : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_cGenerateaAdapter : public cGenerateaAdapter
  {
  public:
    // add here members for free access.
    using cGenerateaAdapter::cGenerateaAdapter; // delegate constructors
  };

};
 
TEST_F(test_cGenerateaAdapter, test_ctor )
{
  Test_cGenerateaAdapter t;
}

