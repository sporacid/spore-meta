#pragma once

#include "spore/meta/meta_enabled.hpp"
#include "spore/meta/meta_string_convert.hpp"

namespace spore::meta::strings
{
    template <typename value_t>
    concept cv_qualified = std::is_const_v<value_t> or std::is_volatile_v<value_t>;

    template <std::signed_integral auto value_v>
    consteval any_meta_string auto to_string(const meta_string_adl_value<value_v>)
    {
        using value_t = decltype(value_v);

        if constexpr (value_v < 0)
        {
            return "-" + to_string(static_cast<std::make_unsigned_t<value_t>>(-value_v));
        }
        else
        {
            return to_string(static_cast<std::make_unsigned_t<value_t>>(value_v));
        }
    }

    template <std::unsigned_integral auto value_v>
    consteval any_meta_string auto to_string(const meta_string_adl_value<value_v>)
    {
        using value_t = decltype(value_v);

        if constexpr (value_v == 0)
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
                return to_string<remainder>() + chars;
            }
            else
            {
                return meta_string {chars};
            }
        }
    }

    template <char value_v>
    consteval any_meta_string auto to_string(const meta_string_adl_value<value_v>)
    {
        constexpr char chars[] {'\'', value_v, '\'', '\0'};
        return meta_string {chars};
    }

    template <std::size_t size_v, char value_v[size_v]>
    consteval any_meta_string auto to_string(const meta_string_adl_value<value_v>)
    {
        return meta_string {value_v};
    }

    template <any_meta_string auto value_v>
    consteval any_meta_string auto to_string(const meta_string_adl_value<value_v>)
    {
        return value_v;
    }

    template <std::size_t index_v, meta_string separator, auto... values_v>
    consteval any_meta_string auto to_string(const meta_string_adl_value<values_v>...)
    {
        if constexpr (index_v < sizeof...(values_v))
        {
            constexpr auto value = std::get<index_v>(std::tuple(values_v...));

            if constexpr (index_v < sizeof...(values_v) - 1)
            {
                return to_string<value>() + separator + to_string<index_v + 1, separator>(meta_string_adl_value<values_v> {}...);
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

    consteval any_meta_string auto to_string(const meta_string_adl_type<void>)
    {
        return meta_string {"void"};
    }

    template <meta_enabled value_t>
    consteval any_meta_string auto to_string(const meta_string_adl_type<value_t>)
    {
        constexpr meta_adl<value_t> adl;
        constexpr auto func = []<meta_type type_v> { return type_v.name; };
        return with_meta_type(adl, func);
    }

    template <cv_qualified value_t>
    consteval any_meta_string auto to_string(const meta_string_adl_type<value_t>)
    {
        constexpr any_meta_string auto base_name = to_string<std::remove_cv_t<value_t>>();

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
            return meta_string {""};
        }
    }

    template <typename value_t>
    consteval any_meta_string auto to_string(const meta_string_adl_type<value_t&>)
    {
        return to_string<value_t>() + "&";
    }

    template <typename value_t>
    consteval any_meta_string auto to_string(const meta_string_adl_type<value_t&&>)
    {
        return to_string<value_t>() + "&&";
    }

    template <typename value_t>
    consteval any_meta_string auto to_string(const meta_string_adl_type<value_t*>)
    {
        return to_string<value_t>() + "*";
    }

    template <typename value_t>
    consteval any_meta_string auto to_string(const meta_string_adl_type<value_t[]>)
    {
        return to_string<value_t>() + "[]";
    }

    template <typename value_t, std::size_t size_v>
    consteval any_meta_string auto to_string(const meta_string_adl_type<value_t[size_v]>)
    {
        return to_string<value_t>() + "[" + to_string<size_v>() + "]";
    }

    template <typename return_t, typename... args_t>
    consteval any_meta_string auto to_string(const meta_string_adl_type<return_t(args_t...)>)
    {
        return to_string<return_t>() + "(" + to_string<", ", args_t...>() + ")";
    }

    template <typename return_t, typename... args_t>
    consteval any_meta_string auto to_string(const meta_string_adl_type<return_t (*)(args_t...)>)
    {
        return to_string<return_t>() + "(*)(" + to_string<", ", args_t...>() + ")";
    }

    template <typename return_t, typename... args_t>
    consteval any_meta_string auto to_string(const meta_string_adl_type<return_t (&)(args_t...)>)
    {
        return to_string<return_t>() + "(&)(" + to_string<", ", args_t...>() + ")";
    }

    template <typename this_t, typename return_t, typename... args_t>
    consteval any_meta_string auto to_string(const meta_string_adl_type<return_t (this_t::*)(args_t...)>)
    {
        return to_string<return_t>() + "(" + to_string<this_t>() + "::*)(" + to_string<", ", args_t...>() + ")";
    }

    template <std::size_t index_v, meta_string separator, typename... values_t>
    consteval any_meta_string auto to_string(const meta_string_adl_type<values_t>...)
    {
        if constexpr (index_v < sizeof...(values_t))
        {
            using value_t = std::tuple_element_t<index_v, std::tuple<values_t...>>;

            if constexpr (index_v < sizeof...(values_t) - 1)
            {
                return to_string<value_t>() + separator + to_string<index_v + 1, separator>(meta_string_adl_type<values_t> {}...);
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
}