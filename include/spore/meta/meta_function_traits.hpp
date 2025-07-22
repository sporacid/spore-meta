#pragma once

#include <functional>

namespace spore
{
    template <typename function_t>
    struct meta_function_traits;

    template <typename return_t, typename... args_t>
    struct meta_function_traits_base
    {
        using return_type = return_t;
        using args_type = std::tuple<args_t...>;
    };

    template <typename return_t, typename this_t, typename... args_t>
    struct meta_function_traits<return_t (this_t::*)(args_t...)> : meta_function_traits_base<return_t, this_t, args_t...>
    {
    };

    template <typename return_t, typename this_t, typename... args_t>
    struct meta_function_traits<return_t (this_t::*)(args_t...) const> : meta_function_traits_base<return_t, this_t, args_t...>
    {
    };

    template <typename return_t, typename this_t, typename... args_t>
    struct meta_function_traits<return_t (this_t::*)(args_t...) noexcept> : meta_function_traits_base<return_t, this_t, args_t...>
    {
    };

    template <typename return_t, typename this_t, typename... args_t>
    struct meta_function_traits<return_t (this_t::*)(args_t...) const noexcept> : meta_function_traits_base<return_t, this_t, args_t...>
    {
    };

    template <typename return_t, typename... args_t>
    struct meta_function_traits<return_t (*)(args_t...)> : meta_function_traits_base<return_t, args_t...>
    {
    };

    template <typename return_t, typename... args_t>
    struct meta_function_traits<return_t (*)(args_t...) noexcept> : meta_function_traits_base<return_t, args_t...>
    {
    };

    template <typename return_t, typename... args_t>
    struct meta_function_traits<std::function<return_t(args_t...)>> : meta_function_traits_base<return_t, args_t...>
    {
    };

    template <typename return_t, typename... args_t>
    struct meta_function_traits<std::function<return_t(args_t...) noexcept>> : meta_function_traits_base<return_t, args_t...>
    {
    };
}