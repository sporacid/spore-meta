#include "spore/examples/adl.hpp"

#include <iostream>

int main()
{
    using namespace spore;

#ifdef SPORE_WITH_TYPE_REGISTRATION
    meta::register_types();
#endif

    std::cout << meta::get_name<ns1::t1>() << std::endl;
    std::cout << meta::get_name<ns2::t2>() << std::endl;
    std::cout << meta::get_name<ns2::ns3::t3<int>>() << std::endl;

    return 0;
}