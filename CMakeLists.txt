cmake_minimum_required(VERSION 3.14)
project(ArchitectureAndDesignPatterns_HW6_AdapterAndBridge)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()

add_executable(
  AdapterAndBridge

  source/cgenerateadapter.cpp
  source/cgenerateadapter.hpp
  source/cvector.hpp
  source/ispaceship.hpp
  source/ccppfunctiondeclarationparser.hpp
  source/ccppfunctiondeclarationparser.cpp
  source/testInterfaceFile.cpp
  source/testInterfaceFile.hpp

  source/tests/test_cCppFunctionDeclarationParser.cpp
  source/tests/test_cGenerateAdapter.cpp
)

target_include_directories(
   AdapterAndBridge PRIVATE 
   source
)

target_link_libraries(
  AdapterAndBridge
  GTest::gtest_main
  GTest::gmock_main
)

include(GoogleTest GoogleMock)
gtest_discover_tests(AdapterAndBridge)