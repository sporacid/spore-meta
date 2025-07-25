#pragma once

#include <concepts>
#include <limits>
#include <string>

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
    // template <>
    struct meta_truthy<value_t>
    {
        static constexpr bool is_truthy(const std::string_view value)
        {
            constexpr char false_string[] = "false";
            constexpr auto predicate = [](const char c1, const char c2) { return std::tolower(c1) == std::tolower(c2); };
            return not value.empty() and not std::equal(value.begin(), value.end(), false_string, predicate);
        }

        // template <std::size_t size_v>
        // static constexpr bool is_truthy(const char (&value)[size_v])
        // {
        //     return is_truthy(std::string_view(value, size_v));
        // }
    };

    // template <std::size_t size_v>
    // struct meta_truthy<char[size_v]>
    // {
    //     static constexpr bool is_truthy(const char (&value)[size_v])
    //     {
    //         return meta_truthy<std::string_view>::is_truthy(std::string_view(value, size_v));
    //     }
    // };

    // template <>
    // struct meta_truthy<char*>
    // {
    //     static constexpr bool is_truthy(const char* value)
    //     {
    //         return meta_truthy<std::string_view>::is_truthy(std::string_view(value));
    //     }
    // };

    namespace meta
    {
        template <typename value_t>
        constexpr bool is_truthy(const value_t& value)
        {
            return meta_truthy<value_t>::is_truthy(value);
        }
    }
}