#pragma once

#include "spore/meta/meta_adl.hpp"

#include <concepts>

namespace spore
{
    namespace detail
    {
        template <auto value_v>
        struct is_literal_type
        {
        };
    }

    // clang-format off
    template <typename value_t>
    concept any_meta_extension = requires(value_t value)
    {
        { detail::is_literal_type<value_t {}> {} };
        { []() consteval { return make_extension(meta_adl<value_t> {}, meta_adl<int> {}); }() } -> std::same_as<value_t>;
    };
    // clang-format on
}