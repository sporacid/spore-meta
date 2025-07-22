#pragma once

namespace spore
{
    template <typename value_t>
    struct meta_field_traits;

    template <typename this_t, typename value_t>
    struct meta_field_traits<value_t this_t::*>
    {
        using this_type = this_t;
        using value_type = value_t;
    };
}