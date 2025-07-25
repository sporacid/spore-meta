#pragma once

#include "spore/meta/meta_truthy.hpp"

#include <cstddef>
#include <type_traits>

namespace spore
{
    template <std::size_t name_v, typename value_t>
    struct meta_attribute
    {
        const char name[name_v];
        value_t value;

        constexpr bool is_truthy() const
        {
            return meta_truthy<value_t>::is_truthy(value);
        }
    };

    template <std::size_t name_v, typename value_t>
    meta_attribute(const char (&)[name_v], value_t)
        -> meta_attribute<name_v, value_t>;

    template <std::size_t name_v, std::size_t value_v>
    meta_attribute(const char (&)[name_v], const char (&)[value_v])
        -> meta_attribute<name_v, const char[value_v]>;

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