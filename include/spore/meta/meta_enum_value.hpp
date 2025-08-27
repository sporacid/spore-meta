#pragma once

#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_tuple.hpp"

#include <cstddef>
#include <type_traits>

namespace spore
{
    template <std::size_t name_v, typename value_t, any_meta_tuple_of<is_meta_attribute>... attributes_t>
    struct meta_enum_value;

    template <std::size_t name_v, typename value_t, any_meta_attribute... attributes_t>
    struct meta_enum_value<name_v, value_t, meta_tuple<attributes_t...>>
    {
        meta_string<name_v> name;
        value_t value;
        meta_tuple<attributes_t...> attributes;
    };

    template <std::size_t name_v, typename value_t, any_meta_attribute... attributes_t>
    meta_enum_value(meta_string<name_v>, value_t, meta_tuple<attributes_t...>)
        -> meta_enum_value<name_v, value_t, meta_tuple<attributes_t...>>;

    template <typename>
    struct is_meta_enum_value : std::false_type
    {
    };

    template <std::size_t name_v, typename value_t, any_meta_attribute... attributes_t>
    struct is_meta_enum_value<meta_enum_value<name_v, value_t, meta_tuple<attributes_t...>>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_enum_value_v = is_meta_enum_value<value_t>::value;

    template <typename value_t>
    concept any_meta_enum_value = is_meta_enum_value_v<value_t>;
}