#pragma once

#include "spore/meta/meta_enum.hpp"
#include "spore/meta/meta_type.hpp"
#include "spore/meta/meta_utils.hpp"

#include <cstdint>
#include <functional>

namespace spore
{
    struct meta_type_id
    {
        std::uint64_t hash : 63 = 0;
        std::uint64_t set : 1 = 0;

        constexpr meta_type_id() = default;

        constexpr meta_type_id(const std::string_view type_name)
        {
            hash = meta::utils::hash_string(type_name);
            set = true;
        }

        template <any_meta_type value_t>
        constexpr meta_type_id(const value_t& value)
            : meta_type_id(value.name)
        {
        }

        template <any_meta_enum value_t>
        constexpr meta_type_id(const value_t& value)
            : meta_type_id(value.name)
        {
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

template <>
struct std::hash<spore::meta_type_id>
{
    constexpr std::size_t operator()(const spore::meta_type_id& value) const noexcept
    {
        return value.set ? (value.hash << 1) + 1 : 0;
    }
};