#include "spore/examples/json.hpp"

#include <iostream>

#include "spore/meta/codegen/meta_codegen_detail.hpp"
#include "spore/meta/codegen/meta_codegen_utils.hpp"
#include "spore/meta/meta_name.hpp"

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

    return 0;
}