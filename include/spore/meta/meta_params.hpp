#pragma once

#include <type_traits>

#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_type_ref.hpp"

namespace spore
{
    template <std::size_t name_v, std::size_t type_v>
    struct meta_type_param
    {
        meta_string<name_v> name;
        meta_string<type_v> type;
        bool variadic = false;
    };

    template <std::size_t name_v, std::size_t type_v>
    meta_type_param(meta_string<name_v>, meta_string<type_v>, ...)
        -> meta_type_param<name_v, type_v>;

    template <std::size_t name_v, typename value_t>
    struct meta_value_param
    {
        using value_type = value_t;

        meta_string<name_v> name;
        meta_type_ref<value_t> type;
        bool variadic = false;
    };

    template <std::size_t name_v, typename value_t>
    meta_value_param(meta_string<name_v>, meta_type_ref<value_t>, ...)
        -> meta_value_param<name_v, value_t>;

    template <std::size_t index_v>
    struct meta_param_ref
    {
        static constexpr std::size_t index = index_v;
    };

    template <typename>
    struct is_meta_param : std::false_type
    {
    };

    template <std::size_t name_v, std::size_t type_v>
    struct is_meta_param<meta_type_param<name_v, type_v>> : std::true_type
    {
    };

    template <std::size_t name_v, typename value_t>
    struct is_meta_param<meta_value_param<name_v, value_t>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_param_v = is_meta_param<value_t>::value;

    template <typename value_t>
    concept any_meta_param = is_meta_param_v<value_t>;
}