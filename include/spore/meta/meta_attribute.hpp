#pragma once

#include <type_traits>

namespace spore
{
    template <size_t name_v, typename value_t>
    struct meta_attribute
    {
        const char name[name_v];
        value_t value;
    };

    template <size_t name_v, typename value_t>
    meta_attribute(const char (&)[name_v], value_t)
        -> meta_attribute<name_v, value_t>;

    template <size_t name_v, size_t value_v>
    meta_attribute(const char (&)[name_v], const char (&)[value_v])
        -> meta_attribute<name_v, const char[value_v]>;

    template <typename>
    struct is_meta_attribute : std::false_type
    {
    };

    template <size_t name_v, typename value_t>
    struct is_meta_attribute<meta_attribute<name_v, value_t>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_attribute_v = is_meta_attribute<value_t>::value;

    template <typename attribute_t>
    concept any_meta_attribute = is_meta_attribute_v<attribute_t>;
}