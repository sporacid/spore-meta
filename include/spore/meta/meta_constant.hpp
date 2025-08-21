#pragma once

#include <type_traits>

namespace spore
{
    template <auto value_v>
    struct meta_constant
    {
        static constexpr auto value = value_v;
    };

    template <typename value_t>
    struct is_meta_constant : std::false_type
    {
    };

    template <auto value_v>
    struct is_meta_constant<meta_constant<value_v>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_constant_v = is_meta_constant<value_t>::value;

    // clang-format off
    template <typename value_t, typename expected_value_t = void>
    concept any_meta_constant = is_meta_constant_v<value_t>
        and (std::is_same_v<expected_value_t, void> or
             std::is_convertible_v<decltype(value_t::value), expected_value_t>);
    // clang-format on
}