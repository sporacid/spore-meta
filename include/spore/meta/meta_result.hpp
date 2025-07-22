#pragma once

#include <type_traits>

namespace spore
{
    struct meta_continue
    {
    };

    struct meta_break
    {
    };

    template <typename value_t>
    struct meta_return
    {
        value_t value;

        constexpr operator const value_t&() const
        {
            return value;
        }

        constexpr operator value_t&() &
        {
            return value;
        }

        constexpr operator value_t&&() &&
        {
            return value;
        }
    };

    template <typename value_t>
    struct is_meta_continue : std::false_type
    {
    };

    template <>
    struct is_meta_continue<meta_continue> : std::true_type
    {
    };

    template <typename value_t>
    struct is_meta_break : std::false_type
    {
    };

    template <>
    struct is_meta_break<meta_break> : std::true_type
    {
    };

    template <typename value_t>
    struct is_meta_return : std::false_type
    {
    };

    template <typename value_t>
    struct is_meta_return<meta_return<value_t>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_continue_v = is_meta_continue<value_t>::value;

    template <typename value_t>
    constexpr bool is_meta_break_v = is_meta_break<value_t>::value;

    template <typename value_t>
    constexpr bool is_meta_return_v = is_meta_return<value_t>::value;

    template <typename value_t>
    constexpr bool is_meta_result_v =
        is_meta_continue_v<value_t> or
        is_meta_break_v<value_t> or
        is_meta_return_v<value_t>;

    template <typename value_t>
    concept meta_result = is_meta_result_v<value_t>;

    template <meta_result result_t, meta_result other_result_t>
    constexpr bool operator==(const result_t& result, const other_result_t& other_result)
    {
        if constexpr (is_meta_continue_v<result_t> and is_meta_continue_v<other_result_t>)
        {
            return true;
        }
        else if constexpr (is_meta_break_v<result_t> and is_meta_break_v<other_result_t>)
        {
            return true;
        }
        else if constexpr (is_meta_return_v<result_t> and is_meta_return_v<other_result_t>)
        {
            return result.value == other_result.value;
        }
        else
        {
            return false;
        }
    }
}