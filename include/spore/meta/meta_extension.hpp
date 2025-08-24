#pragma once

#include "spore/meta/meta_adl.hpp"

#include <concepts>

namespace spore
{
    namespace detail
    {
        template <auto>
        struct is_literal_type
        {
        };
    }

    // clang-format off
    template <typename value_t>
    concept any_meta_extension = requires(value_t value)
    {
        { detail::is_literal_type<value_t {}> {} };
    };
    // clang-format on
}