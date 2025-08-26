#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_enum.hpp"
#include "spore/meta/meta_type.hpp"

#include <type_traits>

namespace spore
{
    // clang-format off
    template <typename value_t>
    concept meta_type_enabled = requires(value_t value)
    {
        { get_meta_type(meta_adl<value_t> {}) } -> any_meta_type;
    };

    template <typename value_t>
    concept meta_enum_enabled = std::is_enum_v<value_t> && requires(value_t value)
    {
        { get_meta_enum(meta_adl<value_t> {}) } -> any_meta_enum;
    };
    // clang-format on

    template <typename value_t>
    concept meta_enabled = meta_type_enabled<value_t> or meta_enum_enabled<value_t>;
}