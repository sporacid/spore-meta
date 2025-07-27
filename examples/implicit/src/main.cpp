#include "spore/examples/implicit.hpp"

#include <iostream>

int main()
{
    using namespace spore;
    using namespace spore::examples::implicit;

    my_struct s {42, 3.14159f};

    std::cout << "For each fields: " << std::endl;

    meta::for_each_field(s, [&]<meta_field field_v> {
        std::cout << "  " << field_v.name << ": " << field_v.get(s) << std::endl;
    });

    std::cout << std::endl;
    std::cout << "For each functions: " << std::endl;

    meta::for_each_function(s, [&]<meta_function function_v> {
        std::cout << "  " << function_v.name << ": " << function_v.invoke(s) << std::endl;
    });

    std::cout << std::endl;
    std::cout << "For each enum values: " << std::endl;

    meta::for_each_value<my_enum>([]<meta_enum_value value_v> {
        std::cout << "  " << value_v.name << ": " << value_v.value << std::endl;
    });

    return 0;
}