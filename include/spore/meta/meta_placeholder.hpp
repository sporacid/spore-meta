#pragma once

namespace spore
{
    template <bool variadic_v>
    struct meta_placeholder
    {
        static constexpr bool variadic = variadic_v;
    };

    template <typename value_t>
    struct is_meta_placeholder : std::false_type
    {
    };

    template <bool variadic_v>
    struct is_meta_placeholder<meta_placeholder<variadic_v>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_placeholder_v = is_meta_placeholder<value_t>::value;

    template <typename value_t>
    concept any_meta_placeholder = is_meta_placeholder_v<value_t>;
}