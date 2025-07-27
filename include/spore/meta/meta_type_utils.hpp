#pragma once

#include "spore/meta/meta_enabled.hpp"
#include "spore/meta/meta_string.hpp"

#include <optional>
#include <tuple>

namespace spore::meta::types
{
    template <typename value_t>
    consteval any_meta_string auto to_string();

    template <auto value_v>
    consteval any_meta_string auto to_string();

    template <meta_string separator, typename... values_t>
    consteval any_meta_string auto to_string();

    template <meta_string separator, auto... values_v>
    consteval any_meta_string auto to_string();

    namespace detail
    {
        template <typename value_t>
        concept cv_qualified = std::is_const_v<value_t> or std::is_volatile_v<value_t>;

        template <meta_enabled value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t>);

        template <cv_qualified value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t>);

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t&>);

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t&&>);

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t*>);

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t[]>);

        template <typename value_t, std::size_t size_v>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t[size_v]>);

        template <std::size_t index_v, meta_string separator, auto... values_v>
        consteval any_meta_string auto to_string_impl()
        {
            if constexpr (index_v < sizeof...(values_v))
            {
                constexpr auto value = std::get<index_v>(std::tuple(values_v...));

                if constexpr (index_v < sizeof...(values_v) - 1)
                {
                    return to_string<value>() + separator + to_string_impl<index_v + 1, separator, values_v...>();
                }
                else
                {
                    return to_string<value>();
                }
            }
            else
            {
                return meta_string {""};
            }
        }

        template <std::size_t index_v, meta_string separator, typename... values_t>
        consteval any_meta_string auto to_string_impl()
        {
            if constexpr (index_v < sizeof...(values_t))
            {
                constexpr auto value = std::get<index_v>(std::tuple(meta_type_ref<values_t> {}...));

                using value_t = typename decltype(value)::value_type;

                if constexpr (index_v < sizeof...(values_t) - 1)
                {
                    return to_string<value_t>() + separator + to_string_impl<index_v + 1, separator, values_t...>();
                }
                else
                {
                    return to_string<value_t>();
                }
            }
            else
            {
                return meta_string {""};
            }
        }

        template <std::integral auto value_v, std::uint8_t... digits_v>
        consteval auto to_digits_impl(std::integer_sequence<std::uint8_t, digits_v...> = {})
        {
            if constexpr (value_v < 0)
            {
                return to_digits_impl<-value_v, digits_v...>();
            }
            else if constexpr (value_v > 0)
            {
                constexpr std::uint8_t digit = static_cast<std::uint8_t>(value_v % 10);
                constexpr std::integral auto remainder = value_v / 10;

                return to_digits_impl<remainder, digit, digits_v...>();
            }
            else
            {
                return std::integer_sequence<std::uint8_t, digits_v...> {};
            }
        }

        template <std::integral auto value_v>
        consteval any_meta_string auto to_string_impl()
        {
            using value_t = decltype(value_v);

            if constexpr (std::is_same_v<char, value_t> or std::is_same_v<unsigned char, value_t>)
            {
                constexpr char chars[] {'\'', static_cast<char>(value_v), '\'', '\0'};
                return meta_string {chars};
            }
            else if constexpr (std::is_signed_v<value_t> and value_v < 0)
            {
                return "-" + to_string_impl<-value_v>();
            }
            else if constexpr (value_v == 0)
            {
                return meta_string {"0"};
            }
            else
            {
                constexpr value_t digit = value_v % 10;
                constexpr value_t remainder = value_v / 10;

                constexpr char chars[] = {static_cast<char>('0' + digit), '\0'};

                if constexpr (remainder > 0)
                {
                    return to_string_impl<remainder>() + chars;
                }
                else
                {
                    return meta_string {chars};
                }
            }
        }

        template <meta_string value_v>
        consteval any_meta_string auto to_string_impl()
        {
            return value_v;
        }

        consteval any_meta_string auto to_string_impl(const meta_type_ref<void>)
        {
            return meta_string {"void"};
        }

        template <meta_enabled value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t>)
        {
            constexpr meta_adl<value_t> adl;
            constexpr auto func = []<meta_type type_v> { return type_v.name; };
            return with_meta_type(adl, func);
        }

        template <cv_qualified value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t>)
        {
            constexpr any_meta_string auto base_name = to_string_impl(meta_type_ref<std::remove_cv_t<value_t>> {});

            if constexpr (std::is_const_v<value_t> and std::is_volatile_v<value_t>)
            {
                return "const volatile " + base_name;
            }
            else if constexpr (std::is_const_v<value_t>)
            {
                return "const " + base_name;
            }
            else if constexpr (std::is_volatile_v<value_t>)
            {
                return "volatile " + base_name;
            }
            else
            {
                static_assert(false);
            }
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t&>)
        {
            return to_string<value_t>() + "&";
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t&&>)
        {
            return to_string<value_t>() + "&&";
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t*>)
        {
            return to_string<value_t>() + "*";
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t[]>)
        {
            return to_string<value_t>() + "[]";
        }

        template <typename value_t, std::size_t size_v>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t[size_v]>)
        {
            return to_string<value_t>() + "[" + to_string<size_v>() + "]";
        }

        template <typename return_t, typename... args_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<return_t(args_t...)>)
        {
            return to_string<return_t>() + "(" + to_string<", ", args_t...>() + ")";
        }

        template <typename return_t, typename... args_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<return_t (*)(args_t...)>)
        {
            return to_string<return_t>() + "(*)(" + to_string<", ", args_t...>() + ")";
        }

        template <typename return_t, typename... args_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<return_t (&)(args_t...)>)
        {
            return to_string<return_t>() + "(&)(" + to_string<", ", args_t...>() + ")";
        }

        template <typename this_t, typename return_t, typename... args_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<return_t (this_t::*)(args_t...)>)
        {
            return to_string<return_t>() + "(" + to_string<this_t>() + "::*)(" + to_string<", ", args_t...>() + ")";
        }

        template <std::size_t index_v, std::size_t capacity_v>
        constexpr std::size_t to_hash_impl(const meta_string<capacity_v>& string, std::size_t hash = 14695981039346656037ull)
        {
            // From https://github.com/foonathan/string_id

            if constexpr (index_v < meta_string<capacity_v>::size())
            {
                constexpr std::size_t fnv_prime = 1099511628211ull;
                const std::size_t new_hash = (hash ^ string[index_v]) * fnv_prime;
                return to_hash_impl<index_v + 1>(string, new_hash);
            }
            else
            {
                return hash;
            }
        }
    }

    template <typename value_t>
    consteval any_meta_string auto to_string()
    {
        return detail::to_string_impl(meta_type_ref<value_t> {});
    }

    template <auto value_v>
    consteval any_meta_string auto to_string()
    {
        if constexpr (any_meta_type_ref<decltype(value_v)>)
        {
            return detail::to_string_impl(value_v);
        }
        else
        {
            return detail::to_string_impl<value_v>();
        }
    }

    template <meta_string separator, typename... values_t>
    consteval any_meta_string auto to_string()
    {
        return detail::to_string_impl<0, separator, values_t...>();
    }

    template <meta_string separator, auto... values_v>
    consteval any_meta_string auto to_string()
    {
        return detail::to_string_impl<0, separator, values_v...>();
    }

    template <std::size_t size_v>
    constexpr std::size_t to_hash(const meta_string<size_v>& string)
    {
        return detail::to_hash_impl<0>(string);
    }
}