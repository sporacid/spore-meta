#include "spore/examples/json.hpp"

#include <iostream>

#include "spore/meta/codegen/meta_codegen_detail.hpp"
#include "spore/meta/codegen/meta_codegen_utils.hpp"
#include "spore/meta/extra/meta_std_types.hpp"

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

    static_assert(meta_type_enabled<message>);

    meta::for_each_field<message>([&]<meta_field field_v> {
        std::cout << field_v.name << ": " << field_v.get(parsed) << std::endl;
    });

    constexpr meta_string wtf = meta::codegen::detail::meta_name_impl<const std::vector<message>&>::get();

    // constexpr auto wtf = meta::codegen::detail::meta_name_impl<const message&>::get();
//
    // constexpr auto wtd = meta::strings::concat(meta_string {"avc"}, meta_string {"123"}, meta_string{ "trololol"});

    // constexpr auto name1 = "std::vector" +
    //                        meta::codegen::detail::get_param_names<meta_type_ref<std::int64_t> {}, meta_type_ref<std::allocator<std::int64_t>> {}>();
    //
    // constexpr auto name2 = "std::array" +
    //                        meta::codegen::detail::get_param_names<meta_type_ref<std::int64_t> {}, std::size_t {64}>();
    //
    // constexpr auto name3 = meta::get_name<std::uint8_t>();

    // constexpr auto name = ""; // meta::codegen::get_name("std::vector", "std::int64_t", "std::allocator<std::int64_t>");
    // meta::codegen::get_name("std::vector",
    //      meta::codegen::get_param_name<std::int64_t>(),
    //      meta::codegen::get_param_name<std::allocator<std::int64_t>>());

    // constexpr auto str = meta::codegen::utils::to_string<1234>();

    return 0;
}