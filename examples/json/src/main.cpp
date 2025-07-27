#include "spore/examples/json.hpp"

#include <iostream>

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

    std::cout << meta::types::to_string<", ", 0, 42, 42ULL, 42LL, 42L, 42U, 12471284184712947194ULL, -124124142LL, 'a', 'b'>() << std::endl;
    constexpr auto a = meta::types::detail::to_digits<4124124>();
    constexpr auto b  = meta::types::detail::to_digits<-4124124>();

    return 0;
}