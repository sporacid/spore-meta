#pragma once

#include "spore/meta/meta_enum.hpp"
#include "spore/meta/meta_type.hpp"
#include "spore/meta/meta_utils.hpp"

#include <cstdint>
#include <functional>

#ifdef SPORE_WITH_STD_FORMATTERS
#    include <format>
#endif

namespace spore
{
    struct meta_type_id
    {
        std::uint64_t set : 1 = 0;
        std::uint64_t hash : 63 = 0;

        constexpr meta_type_id() = default;

        constexpr meta_type_id(const std::string_view type_name)
        {
            set = true;
            hash = meta::utils::hash_string(type_name);
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

        constexpr std::uint64_t value() const
        {
            return set ? (hash << 1) + 1 : 0;
        }

        constexpr operator bool() const
        {
            return valid();
        }

        constexpr operator std::uint64_t() const
        {
            return value();
        }

        constexpr bool operator==(const meta_type_id& other) const
        {
            return std::tie(set, hash) == std::tie(other.set, other.hash);
        }

        constexpr bool operator<(const meta_type_id& other) const
        {
            return std::tie(set, hash) < std::tie(other.set, other.hash);
        }
    };
}

template <>
struct std::hash<spore::meta_type_id>
{
    constexpr std::size_t operator()(const spore::meta_type_id& value) const noexcept
    {
        return value.value();
    }
};

#ifdef SPORE_WITH_STD_FORMATTERS
template <>
struct std::formatter<spore::meta_type_id> : std::formatter<std::uint64_t>
{
    using std::formatter<std::uint64_t>::parse;

    auto format(const spore::meta_type_id& value, std::format_context& ctx) const
    {
        return std::formatter<std::uint64_t>::format(value.value(), ctx);
    }
};
#endif