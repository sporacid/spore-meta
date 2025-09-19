#pragma once

#include <cstdint>
#include <string_view>

namespace spore::meta::utils
{
    namespace detail
    {
        template <auto arg_v, typename func_t>
        constexpr auto is_constexpr_invocable_impl(func_t func)
            -> std::integral_constant<bool, (func.template operator()<arg_v>(), true)>
        {
            return {};
        }

        template <auto arg_v>
        constexpr std::false_type is_constexpr_invocable_impl(...)
        {
            return {};
        }
    }

    template <typename func_t, auto arg_v>
    consteval bool is_constexpr_invocable()
    {
        if constexpr (std::is_default_constructible_v<func_t>)
        {
            constexpr auto value = detail::is_constexpr_invocable_impl<arg_v>(func_t {});
            return decltype(value)::value;
        }
        else
        {
            return false;
        }
    }

    constexpr std::uint64_t hash_string(const std::string_view string)
    {
        // From https://github.com/foonathan/string_id

        constexpr std::uint64_t initial_hash = 14695981039346656037ULL;

        std::uint64_t hash = initial_hash;

        for (const char c : string)
        {
            constexpr std::uint64_t fnv_prime = 1099511628211ULL;
            hash = (hash ^ static_cast<std::uint64_t>(c)) * fnv_prime;
        }

        return hash;
    }
}