#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_enum.hpp"
#include "spore/meta/meta_type.hpp"

#include <type_traits>

namespace spore
{
    template <typename value_t>
    concept meta_type_enabled = requires {
        { get_meta_type(meta_adl<value_t> {}) } -> any_meta_type;
    };

    template <typename value_t>
    concept meta_enum_enabled = std::is_enum_v<value_t> and requires {
        { get_meta_enum(meta_adl<value_t> {}) } -> any_meta_enum;
    };

    template <typename value_t>
    concept meta_enabled = meta_type_enabled<value_t> or meta_enum_enabled<value_t>;
}