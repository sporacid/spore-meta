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

    template <meta_string separator, typename... values_t>
    consteval any_meta_string auto to_string()
    {
        return to_string<0, separator>(meta_string_adl_type<values_t> {}...);
    }

    template <meta_string separator, auto... values_v>
    consteval any_meta_string auto to_string()
    {
        return to_string<0, separator>(meta_string_adl_value<values_v> {}...);
    }
}

#include "spore/meta/meta_string_convert.inl"