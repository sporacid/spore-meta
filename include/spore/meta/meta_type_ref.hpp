#pragma once

namespace spore
{
    template <typename... values_t>
    struct meta_type_ref
    {
        using value_type = meta_tuple<values_t...>;
        static constexpr bool variadic = true;
    };

    template <typename value_t>
    struct meta_type_ref<value_t>
    {
        using value_type = value_t;
        static constexpr bool variadic = false;
    };
}