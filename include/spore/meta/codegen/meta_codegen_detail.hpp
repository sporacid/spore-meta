#pragma once

#include "spore/meta/meta.hpp"

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
        static consteval std::string get()
        {
            return "void";
        }
    };

    template <typename value_t>
    requires(std::is_const_v<value_t> or std::is_volatile_v<value_t>) struct meta_name_impl<value_t>
    {
        static consteval std::string get()
        {
            std::string prefix;

            if constexpr (std::is_const_v<value_t>)
            {
                prefix += "const ";
            }

            if constexpr (std::is_volatile_v<value_t>)
            {
                prefix += "volatile ";
            }

            return prefix + meta_name_impl<std::remove_cv_t<value_t>>::get();
        }
    };

    template <typename value_t>
    requires std::is_lvalue_reference_v<value_t> struct meta_name_impl<value_t>
    {
        static consteval std::string get()
        {
            return meta_name_impl<std::remove_reference_t<value_t>>::get() + "&";
        }
    };

    template <typename value_t>
    requires std::is_rvalue_reference_v<value_t> struct meta_name_impl<value_t>
    {
        static consteval std::string get()
        {
            return meta_name_impl<std::remove_reference_t<value_t>>::get() + "&&";
        }
    };

    template <typename value_t>
    requires std::is_pointer_v<value_t> struct meta_name_impl<value_t>
    {
        static consteval std::string get()
        {
            return meta_name_impl<std::remove_pointer_t<value_t>>::get() + "*";
        }
    };

    template <typename value_t, std::size_t size_v>
    struct meta_name_impl<value_t[size_v]>
    {
        static consteval std::string get()
        {
            return meta_name_impl<value_t>::get() + "[" + std::to_string(size_v) + "]";
        }
    };

    template <typename value_t>
    struct meta_name_impl<value_t[]>
    {
        static consteval std::string value()
        {
            return meta_name_impl<value_t>::get() + "[]";
        }
    };

    template <typename result_t, typename... args_t>
    struct meta_name_impl<result_t(args_t...)>
    {
        static consteval std::string value()
        {
            constexpr std::string_view separator = ", ";
            constexpr std::array args {meta_name_impl<args_t>::get()...};

            std::string string = meta_name_impl<result_t>::get() + "(";

            for (const std::string_view& arg : args)
            {
                string += arg;
                string += separator;
            }

            if (string.size() >= separator.size())
            {
                string.resize(string.size() - separator.size());
            }

            string += ")";

            return string;
        }
    };

    template <meta_enabled value_t>
    struct meta_name_impl<value_t>
    {
        static consteval std::string value()
        {
            return std::string(meta::get_name<value_t>());
        }
    };

    template <std::size_t index_v, auto... params_v>
    consteval std::string get_param_name()
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

        get_param_names<params_v>(string, std::index_sequence<indices_v> {});
    }

    template <auto... params_v>
    consteval std::string get_param_names()
    {
        constexpr std::string_view separator = ", ";

        std::string string = "<";

        get_param_names<params_v>(string, std::make_index_sequence<sizeof...(params_v)>());

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

    template <typename value_t>
    consteval std::string get_param_name()
    {
        return "";
    }

    template <auto value_v>
    consteval std::string get_param_name()
    {
        return std::to_string(value_v);
    }
}