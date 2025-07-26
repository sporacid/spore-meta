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

    // constexpr meta_string wtf = meta::codegen::detail::meta_name_impl<const std::vector<message>&>::get();
    // std::cout << wtf << std::endl;

    std::cout << meta::utils::to_string<const std::vector<message>&>() << std::endl;
    std::cout << meta::utils::to_string<volatile const std::array<message, 32>*&&>() << std::endl;
    std::cout << meta::utils::to_string<volatile const std::function<void(int, float)>*&&>() << std::endl;
    return 0;
}