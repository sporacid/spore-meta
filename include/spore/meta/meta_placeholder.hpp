#pragma once

#include <type_traits>

namespace spore
{
    template <bool variadic_v = false>
    struct meta_type_placeholder
    {
        static constexpr bool variadic = variadic_v;
    };

    template <typename value_t, bool variadic_v = false>
    struct meta_value_placeholder
    {
        using value_type = value_t;
        static constexpr bool variadic = variadic_v;
    };

    struct meta_placeholder
    {
        template <typename value_t>
        operator value_t&&() const
        {
            return std::declval<value_t>();
        }
    };
}