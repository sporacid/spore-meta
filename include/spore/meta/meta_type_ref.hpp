#pragma once

#include <type_traits>

namespace spore
{
    template <typename value_t>
    struct meta_type_ref
    {
        using value_type = value_t;
    };

    template <typename>
    struct is_meta_type_ref : std::false_type
    {
    };

    template <typename value_t>
    struct is_meta_type_ref<meta_type_ref<value_t>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_type_ref_v = is_meta_type_ref<value_t>::value;

    template <typename value_t>
    concept any_meta_type_ref = is_meta_type_ref_v<value_t>;
}