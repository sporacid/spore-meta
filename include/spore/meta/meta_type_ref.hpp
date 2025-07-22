#pragma once

namespace spore
{
    template <typename value_t>
    struct meta_type_ref
    {
        using value_type = value_t;
    };
}