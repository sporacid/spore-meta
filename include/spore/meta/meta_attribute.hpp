#pragma once

#include "spore/meta/meta_string.hpp"

#include <cstddef>
#include <type_traits>

namespace spore
{
    template <std::size_t name_v, typename value_t>
    struct meta_attribute
    {
        meta_string<name_v> name;
        value_t value;
    };

    template <std::size_t name_v, typename value_t>
    meta_attribute(meta_string<name_v>, value_t)
        -> meta_attribute<name_v, value_t>;

    template <typename>
    struct is_meta_attribute : std::false_type
    {
    };

    template <std::size_t name_v, typename value_t>
    struct is_meta_attribute<meta_attribute<name_v, value_t>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_attribute_v = is_meta_attribute<value_t>::value;

    template <typename attribute_t>
    concept any_meta_attribute = is_meta_attribute_v<attribute_t>;
}