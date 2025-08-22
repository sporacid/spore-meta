#pragma once

#include "spore/meta/meta_enum.hpp"
#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_type.hpp"
#include "spore/meta/meta_utils.hpp"

#include <cstdint>

namespace spore
{
    struct meta_type_id
    {
        std::uint64_t hash : 63 = 0;
        std::uint64_t set : 1 = 0;

        constexpr meta_type_id() = default;

        template <any_meta_type value_t>
        constexpr meta_type_id(const value_t& value)
        {
            hash = meta::utils::hash_string(value.name);
            set = true;
        }

        template <any_meta_enum value_t>
        constexpr meta_type_id(const value_t& value)
        {
            hash = meta::utils::hash_string(value.name);
            set = true;
        }

        constexpr bool valid() const
        {
            return set;
        }

        constexpr operator bool() const
        {
            return valid();
        }

        constexpr bool operator==(const meta_type_id& other) const
        {
            return std::tie(hash, set) == std::tie(other.hash, other.set);
        }

        constexpr bool operator<(const meta_type_id& other) const
        {
            return std::tie(hash, set) < std::tie(other.hash, other.set);
        }
    };
}
