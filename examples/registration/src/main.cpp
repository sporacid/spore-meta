#include "spore/examples/registration.hpp"
#include "spore/examples/registry.hpp"

#include <iostream>

int main()
{
    using namespace spore;
    using namespace spore::examples::registration;

    meta::register_types();

    const auto action = [](const meta_type_id type_id, const std::string_view type_name) {
        std::cout << "Type registered: " << type_id.hash << " -> " << type_name << std::endl;
    };

    registry::instance().for_each_type(action);

    return 0;
}