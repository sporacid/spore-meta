#pragma once

#include <cstdint>
#include <string_view>

namespace spore::meta::utils
{
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