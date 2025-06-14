#include "cgenerateadapter.hpp"

#include <iostream>

int main(int argc, const char* argv[])
{ 
    std::tuple<int, std::string> r = cGenerateAdapter::main(argc, argv);

    std::cout << std::get<1>(r);

    return std::get<0>(r);
}
