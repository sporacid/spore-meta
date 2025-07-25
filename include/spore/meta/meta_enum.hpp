#pragma once

#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_enum_value.hpp"
#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_tuple.hpp"
#include "spore/meta/meta_type_ref.hpp"

#include <cstddef>
#include <type_traits>

namespace spore
{
    template <std::size_t name_v, typename value_t, typename...>
    struct meta_enum;

    template <std::size_t name_v, typename value_t, any_meta_enum_value... values_t, any_meta_attribute... attributes_t>
    struct meta_enum<name_v, value_t, meta_tuple<values_t...>, meta_tuple<attributes_t...>>
    {
        meta_string<name_v> name;
        meta_type_ref<value_t> type;
        meta_tuple<values_t...> values;
        meta_tuple<attributes_t...> attributes;
    };

    template <std::size_t name_v, typename value_t, any_meta_enum_value... values_t, any_meta_attribute... attributes_t>
    meta_enum(const char (&)[name_v], meta_type_ref<value_t>, meta_tuple<values_t...>, meta_tuple<attributes_t...>)
        -> meta_enum<name_v, value_t, meta_tuple<values_t...>, meta_tuple<attributes_t...>>;

    template <typename>
    struct is_meta_enum : std::false_type
    {
    };

    template <std::size_t name_v, typename value_t, any_meta_enum_value... values_t, any_meta_attribute... attributes_t>
    struct is_meta_enum<meta_enum<name_v, value_t, meta_tuple<values_t...>, meta_tuple<attributes_t...>>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_enum_v = is_meta_enum<value_t>::value;

    template <typename value_t>
    concept any_meta_enum = is_meta_enum_v<value_t>;
}