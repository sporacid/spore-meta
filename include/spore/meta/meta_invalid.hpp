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
        template <typename value_t>
        consteval bool is_valid(value_t&&)
        {
            return not is_meta_invalid_v<std::decay_t<value_t>>;
        }

        template <typename value_t>
        consteval bool is_invalid(value_t&&)
        {
            return is_meta_invalid_v<std::decay_t<value_t>>;
        }
    }
}