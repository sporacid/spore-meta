#pragma once

#include <cstddef>
#include <cstdint>

#include "spore/meta/meta_string.hpp"

namespace spore::meta::utils
{
    namespace detail
    {
        template <std::size_t index_v, std::size_t capacity_v>
        constexpr std::uint64_t hash_string_impl(const meta_string<capacity_v>& string, const std::uint64_t hash = 14695981039346656037ULL)
        {
            // From https://github.com/foonathan/string_id

            if constexpr (index_v < meta_string<capacity_v>::size())
            {
                constexpr std::uint64_t fnv_prime = 1099511628211ULL;
                const std::uint64_t c = static_cast<std::uint64_t>(string[index_v]);
                const std::uint64_t new_hash = (hash ^ c) * fnv_prime;
                return hash_string_impl<index_v + 1>(string, new_hash);
            }
            else
            {
                return hash;
            }
        }
    }

    template <std::size_t capacity_v>
    constexpr std::uint64_t hash_string(const meta_string<capacity_v>& string)
    {
        return detail::hash_string_impl<0>(string);
    }
}