#pragma once

#include "spore/meta/meta.hpp"
#include "spore/meta/meta_string.hpp"

#include <string>
#include <string_view>
#include <type_traits>

namespace spore::meta::codegen::detail
{
    template <typename value_t>
    struct meta_name_impl;

    template <>
    struct meta_name_impl<void>
    {
        static consteval any_meta_string auto get()
        {
            return meta_string {"void"};
        }
    };

    template <typename value_t>
    requires(std::is_const_v<value_t> or std::is_volatile_v<value_t>) struct meta_name_impl<value_t>
    {
        static consteval any_meta_string auto get()
        {
            if constexpr (std::is_const_v<value_t>)
            {
                if constexpr (std::is_volatile_v<value_t>)
                {
                    return strings::concat(meta_string {"const volatile "}, meta_name_impl<std::remove_cv_t<value_t>>::get());
                }
                else
                {
                    return strings::concat(meta_string {"const "}, meta_name_impl<std::remove_cv_t<value_t>>::get());
                }
            }
            else if constexpr (std::is_volatile_v<value_t>)
            {
                return strings::concat(meta_string {"volatile "}, meta_name_impl<std::remove_cv_t<value_t>>::get());
            }
            else
            {
                static_assert(false);
            }
        }
    };

    template <typename value_t>
    requires std::is_lvalue_reference_v<value_t> struct meta_name_impl<value_t>
    {
        static consteval any_meta_string auto get()
        {
            return strings::concat(meta_name_impl<std::remove_reference_t<value_t>>::get(), meta_string {"&"});
        }
    };

    template <typename value_t>
    requires std::is_rvalue_reference_v<value_t> struct meta_name_impl<value_t>
    {
        static consteval any_meta_string auto get()
        {
            return strings::concat(meta_name_impl<std::remove_reference_t<value_t>>::get(), meta_string {"&&"});
        }
    };

    template <typename value_t>
    requires std::is_pointer_v<value_t> struct meta_name_impl<value_t>
    {
        static consteval any_meta_string auto get()
        {
            return strings::concat(meta_name_impl<std::remove_pointer_t<value_t>>::get(), meta_string {"*"});
        }
    };

    template <typename value_t, std::size_t size_v>
    struct meta_name_impl<value_t[size_v]>
    {
        static consteval any_meta_string auto get()
        {
            // TODO @sporacid consteval any_meta_string auto to_string(size_v)
            return strings::concat(meta_name_impl<value_t>::get(), meta_string {"[]"});
        }
    };

    template <typename value_t>
    struct meta_name_impl<value_t[]>
    {
        static consteval any_meta_string auto get()
        {
            return strings::concat(meta_name_impl<value_t>::get(), meta_string {"[]"});
        }
    };

    template <typename result_t, typename... args_t>
    struct meta_name_impl<result_t(args_t...)>
    {
        static consteval any_meta_string auto get()
        {
            // TODO @sporacid this doesn't do what I think it does
            return strings::concat(
                meta_name_impl<result_t>::get(),
                meta_string {"("},
                (meta_name_impl<args_t>::get(), meta_string {", "})...,
                meta_string {")"});
        }
    };

    template <meta_enabled value_t>
    struct meta_name_impl<value_t>
    {
        static consteval any_meta_string auto get()
        {
            return meta::get_name<value_t>();
        }
    };

    template <typename value_t>
    consteval any_meta_string auto get_param_name()
    {
        return meta_name_impl<value_t>::get();
    }

    template <auto value_v>
    consteval any_meta_string auto get_param_name()
    {
        // TODO @sporacid consteval any_meta_string auto to_string(size_v)
        // return std::to_string(value_v);
        return meta_string<1> {};
    }

    template <std::size_t index_v, auto... params_v>
    consteval any_meta_string auto get_param_name()
    {
        using tuple_t = decltype(std::tuple {params_v...});
        using param_t = std::tuple_element_t<index_v, tuple_t>;

        if constexpr (any_meta_type_ref<param_t>)
        {
            return get_param_name<param_t::value_type>();
        }
        else
        {
            constexpr std::tuple params {params_v...};
            constexpr auto param = std::get<index_v>(params);
            return get_param_name<param>();
        }
    }

    template <auto... params_v, std::size_t index_v, std::size_t... indices_v>
    consteval void get_param_names(std::string& string, std::index_sequence<index_v, indices_v...>)
    {
        constexpr std::string_view separator = ", ";

        string += get_param_name<index_v, params_v...>();
        string += separator;

        get_param_names<params_v...>(string, std::index_sequence<indices_v...> {});
    }

    template <auto... params_v>
    consteval std::string get_param_names()
    {
        constexpr std::string_view separator = ", ";

        std::string string = "<";

        get_param_names<params_v...>(string, std::make_index_sequence<sizeof...(params_v)>());

        if (string.size() >= separator.size())
        {
            string.resize(string.size() - separator.size());
        }

        string += ">";

        return string;
#if 0
        constexpr std::string_view separator = ", ";
        constexpr std::tuple params {params_v...};

        std::string string = "<";

        for (const std::string_view& arg : args)
        {
            string += arg;
            string += separator;
        }

        if (string.size() >= separator.size())
        {
            string.resize(string.size() - separator.size());
        }

        string += ">";

        return string;
#endif
    }
}