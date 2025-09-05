#pragma once

#include <type_traits>
#include <variant>

namespace spore
{
    struct meta_invalid
    {
    };

    template <typename>
    struct is_meta_invalid : std::false_type
    {
    };

    template <>
    struct is_meta_invalid<meta_invalid> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_invalid_v = is_meta_invalid<value_t>::value;

    namespace meta
    {
        template <typename value_t>
        consteval bool is_valid()
        {
            return not is_meta_invalid_v<std::decay_t<value_t>>;
        }

        template <typename value_t>
        consteval bool is_valid(value_t&&)
        {
            return is_valid<value_t>();
        }

        template <typename... values_t>
        constexpr bool is_valid(const std::variant<values_t...>& variant)
        {
            constexpr auto visitor = []<typename value_t>(const value_t&) {
                return meta::is_valid<value_t>();
            };
            return std::visit(visitor, variant);
        }

        template <typename value_t>
        consteval bool is_invalid()
        {
            return is_meta_invalid_v<std::decay_t<value_t>>;
        }

        template <typename value_t>
        consteval bool is_invalid(value_t&&)
        {
            return is_invalid<value_t>();
        }

        template <typename... values_t>
        constexpr bool is_invalid(const std::variant<values_t...>& variant)
        {
            constexpr auto visitor = []<typename value_t>(const value_t&) {
                return meta::is_invalid<value_t>();
            };
            return std::visit(visitor, variant);
        }
    }
}