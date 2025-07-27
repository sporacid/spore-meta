#pragma once

#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_tuple.hpp"
#include "spore/meta/meta_type_ref.hpp"

#include <cstddef>
#include <type_traits>

namespace spore
{
    template <std::size_t name_v, typename value_t, any_meta_attribute... attributes_t>
    struct meta_argument
    {
        using value_type = value_t;

        meta_string<name_v> name;
        meta_type_ref<value_t> type;
        meta_tuple<attributes_t...> attributes;
    };

    template <std::size_t name_v, typename value_t, any_meta_attribute... attributes_t>
    meta_argument(meta_string<name_v>, meta_type_ref<value_t>, meta_tuple<attributes_t...>)
        -> meta_argument<name_v, value_t, attributes_t...>;

    template <typename>
    struct is_meta_argument : std::false_type
    {
    };

    template <std::size_t name_v, typename value_t, any_meta_attribute... attributes_t>
    struct is_meta_argument<meta_argument<name_v, value_t, attributes_t...>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_argument_v = is_meta_argument<value_t>::value;

    template <typename value_t>
    concept any_meta_argument = is_meta_argument_v<value_t>;
}