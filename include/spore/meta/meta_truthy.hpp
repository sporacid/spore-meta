#pragma once

#include <algorithm>
#include <cctype>
#include <concepts>
#include <limits>
#include <string_view>

namespace spore
{
    template <typename value_t>
    struct meta_truthy
    {
        static constexpr bool is_truthy(const value_t& value)
        {
            return static_cast<bool>(value);
        }
    };

    template <>
    struct meta_truthy<bool>
    {
        static constexpr bool is_truthy(const bool value)
        {
            return value;
        }
    };

    template <std::integral value_t>
    struct meta_truthy<value_t>
    {
        static constexpr bool is_truthy(const value_t value)
        {
            return value != 0;
        }
    };

    template <std::floating_point value_t>
    struct meta_truthy<value_t>
    {
        static constexpr bool is_truthy(const value_t value)
        {
            constexpr value_t epsilon = std::numeric_limits<value_t>::epsilon();
            return value > epsilon or value < -epsilon;
        }
    };

    template <std::convertible_to<std::string_view> value_t>
    struct meta_truthy<value_t>
    {
        static constexpr bool is_truthy(const std::string_view value)
        {
            constexpr char false_string[] {'f', 'a', 'l', 's', 'e'};
            constexpr auto predicate = [](const char c1, const char c2) { return std::tolower(c1) == std::tolower(c2); };
            return not value.empty() and not std::ranges::equal(value, false_string, predicate);
        }
    };

    namespace meta
    {
        template <typename value_t>
        constexpr bool is_truthy(const value_t& value)
        {
            return meta_truthy<value_t>::is_truthy(value);
        }
    }
}