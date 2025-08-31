#pragma once

#include <type_traits>

#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_type_ref.hpp"

namespace spore
{
    template <std::size_t name_v, std::size_t type_v>
    struct meta_type_parameter
    {
        meta_string<name_v> name;
        meta_string<type_v> type;
        bool variadic = false;
    };

    template <std::size_t name_v, std::size_t type_v>
    meta_type_parameter(meta_string<name_v>, meta_string<type_v>, ...)
        -> meta_type_parameter<name_v, type_v>;

    template <std::size_t name_v, typename value_t>
    struct meta_value_parameter
    {
        meta_string<name_v> name;
        meta_type_ref<value_t> type;
        bool variadic = false;
    };

    template <std::size_t name_v, typename value_t>
    meta_value_parameter(meta_string<name_v>, meta_type_ref<value_t>, ...)
        -> meta_value_parameter<name_v, value_t>;

    template <typename>
    struct is_meta_parameter : std::false_type
    {
    };

    template <std::size_t name_v, std::size_t type_v>
    struct is_meta_parameter<meta_type_parameter<name_v, type_v>> : std::true_type
    {
    };

    template <std::size_t name_v, typename value_t>
    struct is_meta_parameter<meta_value_parameter<name_v, value_t>> : std::true_type
    {
    };

    template <typename value_t>
    concept any_meta_parameter = is_meta_parameter<value_t>::value;

    template <std::size_t index_v>
    struct meta_parameter_ref
    {
        static constexpr std::size_t index = index_v;
    };

    template <typename>
    struct is_meta_parameter_ref : std::false_type
    {
    };

    template <std::size_t index_v>
    struct is_meta_parameter_ref<meta_parameter_ref<index_v>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_parameter_ref_v = is_meta_parameter_ref<value_t>::value;

    template <typename value_t>
    concept any_meta_parameter_ref = is_meta_parameter_ref_v<value_t>;
}