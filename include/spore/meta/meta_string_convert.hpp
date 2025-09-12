#pragma once

#include "spore/meta/meta_string.hpp"

namespace spore::meta::strings
{
    template <typename>
    struct meta_string_adl_type
    {
    };

    template <auto>
    struct meta_string_adl_value
    {
    };

    template <typename value_t>
    consteval any_meta_string auto to_string()
    {
        return to_string(meta_string_adl_type<value_t> {});
    }

    template <auto value_v>
    consteval any_meta_string auto to_string()
    {
        return to_string(meta_string_adl_value<value_v> {});
    }

    template <meta_string separator_v>
    consteval any_meta_string auto join_string()
    {
        return meta_string {""};
    }

    template <meta_string separator_v, auto value_v, auto... values_v>
    consteval any_meta_string auto join_string()
    {
        if constexpr (sizeof...(values_v))
        {
            return to_string<value_v>() + separator_v + join_string<separator_v, values_v...>();
        }
        else
        {
            return to_string<value_v>();
        }
    }

    template <meta_string separator_v, typename value_t, typename... values_t>
    consteval any_meta_string auto join_string()
    {
        if constexpr (sizeof...(values_t))
        {
            return to_string<value_t>() + separator_v + join_string<separator_v, values_t...>();
        }
        else
        {
            return to_string<value_t>();
        }
    }
}

#include "spore/meta/meta_string_convert.inl"