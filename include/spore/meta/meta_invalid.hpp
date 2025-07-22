#pragma once

#include <type_traits>

namespace spore
{
    struct meta_invalid
    {
    };

    template <typename>
    struct is_meta_invalid : std::false_type
    {
    };

    template <>
    struct is_meta_invalid<meta_invalid> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_invalid_v = is_meta_invalid<value_t>::value;

    namespace meta
    {
        template <auto value_v>
        consteval bool is_valid()
        {
            return not is_meta_invalid_v<decltype(value_v)>;
        }

        template <auto value_v>
        consteval bool is_invalid()
        {
            return is_meta_invalid_v<decltype(value_v)>;
        }
    }
}