#pragma once

#include <type_traits>
#include <variant>

namespace spore
{
    struct meta_invalid
    {
    };

    namespace meta
    {
        template <typename value_t>
        constexpr bool is_valid(value_t&&)
        {
            return not std::is_same_v<meta_invalid, std::decay_t<value_t>>;
        }

        template <typename... values_t>
        constexpr bool is_valid(const std::variant<values_t...>& variant)
        {
            return not std::holds_alternative<meta_invalid>(variant);
        }

        template <typename value_t>
        constexpr bool is_invalid(value_t&&)
        {
            return std::is_same_v<meta_invalid, std::decay_t<value_t>>;
        }

        template <typename... values_t>
        constexpr bool is_invalid(const std::variant<values_t...>& variant)
        {
            return std::holds_alternative<meta_invalid>(variant);
        }
    }
}