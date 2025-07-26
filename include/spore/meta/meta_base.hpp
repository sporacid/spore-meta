#pragma once

#include "spore/meta/meta_type_ref.hpp"

#include <type_traits>

namespace spore
{
    template <typename value_t>
    struct meta_base
    {
        using value_type = value_t;

        meta_type_ref<value_t> type;
    };

    template <typename>
    struct is_meta_base : std::false_type
    {
    };

    template <typename value_t>
    struct is_meta_base<meta_base<value_t>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_base_v = is_meta_base<value_t>::value;

    template <typename value_t>
    concept any_meta_base = is_meta_base_v<value_t>;
}