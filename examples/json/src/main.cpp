#include "spore/examples/json.hpp"

#include <iostream>

#include "spore/meta/meta_invocable.hpp"

int main()
{
    using namespace spore;
    using namespace spore::examples::json;

    message msg {
        .id = 1,
        .checksum = 12345,
        .value = "message",
    };

    std::string json = nlohmann::json(msg).dump(2);

    std::cout << "JSON: " << std::endl;
    std::cout << json << std::endl;

    message parsed;
    nlohmann::json::parse(json).get_to(parsed);

    std::cout << std::endl;
    std::cout << "Parsed: " << std::endl;

    meta::for_each_field<message>([&]<meta_field field_v> {
        std::cout << field_v.name << ": " << field_v.get(parsed) << std::endl;
    });

    meta_invocable3 invocable {
        .func = []<typename value_t, int value_v>(int i, float f) {

        },
        .params = meta_tuple {
            meta_type_param {},
            meta_value_param<int> {},
        },
    };

    // meta_invocable<int, meta_constant<42>> invoke = []<typename, > {
    //
    // };

    return 0;
}