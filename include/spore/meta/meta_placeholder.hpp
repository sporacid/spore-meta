#pragma once

namespace spore
{
    struct meta_placeholder
    {
    };

    template <typename value_t>
    struct is_meta_placeholder : std::false_type
    {
    };

    template <>
    struct is_meta_placeholder<meta_placeholder> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_placeholder_v = is_meta_placeholder<value_t>::value;
}