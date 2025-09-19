#include <iostream>

#include "spore/examples/hello_world.hpp"

int main()
{
    using namespace spore;
    using namespace spore::examples::hello_world;

    std::cout << "For each fields: " << std::endl;

    hello_world_fields from_fields;
    meta::for_each_field(from_fields, [&]<meta_field field_v> {
        std::cout << "  " << field_v.name << ": " << field_v.get(from_fields) << std::endl;
    });

    std::cout << std::endl;
    std::cout << "For each functions: " << std::endl;

    hello_world_functions from_functions;
    meta::for_each_function(from_functions, [&]<meta_function function_v> {
        std::cout << "  " << function_v.name << ": " << function_v.invoke(from_functions) << std::endl;
    });

    std::cout << std::endl;
    std::cout << "For each attributes: " << std::endl;

    meta::for_each_attribute<hello_world_attributes>([]<meta_attribute attribute_v> {
        std::cout << "  " << attribute_v.name << ": " << attribute_v.value << std::endl;
    });

    std::cout << std::endl;
    std::cout << "For each enum values: " << std::endl;

    meta::for_each_value<hello_world_enum>([]<meta_enum_value value_v> {
        constexpr auto predicate = []<meta_attribute attribute_v> { return attribute_v.name == "display"; };
        constexpr auto attribute = meta::find_attribute<value_v>(predicate);

        if constexpr (meta::is_valid(attribute))
        {
            std::cout << "  " << value_v.value << ": " << attribute.value << std::endl;
        }
        else
        {
            std::cout << "  " << value_v.value << ": " << value_v.name << std::endl;
        }
    });

    meta::for_each_field_r<impl>([]<meta_field field_v> {
        std::cout << "field: " << field_v.name << std::endl;
    });

    meta::for_each_function_r<impl>([]<meta_function function_v> {
        std::cout << "function: " << function_v.name << std::endl;
    });

    return 0;
}