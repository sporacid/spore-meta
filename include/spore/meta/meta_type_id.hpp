#pragma once

#include "spore/meta/meta_enabled.hpp"
#include "spore/meta/meta_enum.hpp"
#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_type.hpp"
#include "spore/meta/meta_type_utils.hpp"

#include <cstddef>

namespace spore
{
    struct meta_type_id
    {
        std::size_t value = 0;

        constexpr meta_type_id() = default;

        template <any_meta_type type_t>
        constexpr meta_type_id(const type_t& type)
        {
            value = meta::types::to_hash(type.name);
        }

        template <any_meta_enum enum_t>
        constexpr meta_type_id(const enum_t& enum_)
        {
            value = meta::types::to_hash(enum_.name);
        }

        constexpr bool valid() const
        {
            return value > 0;
        }

        constexpr operator bool() const
        {
            return valid();
        }

        constexpr bool operator==(const meta_type_id& other) const
        {
            return value == other.value;
        }

        constexpr bool operator<(const meta_type_id& other) const
        {
            return value < other.value;
        }
    };
}
