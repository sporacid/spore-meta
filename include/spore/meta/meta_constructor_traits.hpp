#pragma once

#include <functional>

namespace spore
{
    template <typename function_t>
    struct meta_constructor_traits;

    template <typename this_t, typename... args_t>
    struct meta_constructor_traits_base
    {
        using this_type = this_t;
        using args_type = std::tuple<args_t...>;
    };

    template <typename this_t, typename... args_t>
    struct meta_constructor_traits<void (*)(this_t&, args_t...)> : meta_constructor_traits_base<this_t, args_t...>
    {
    };

    template <typename this_t, typename... args_t>
    struct meta_constructor_traits<void (*)(this_t&, args_t...) noexcept> : meta_constructor_traits_base<this_t, args_t...>
    {
    };

    template <typename this_t, typename... args_t>
    struct meta_constructor_traits<std::function<void(this_t&, args_t...)>> : meta_constructor_traits_base<this_t, args_t...>
    {
    };

    template <typename this_t, typename... args_t>
    struct meta_constructor_traits<std::function<void(this_t&, args_t...) noexcept>> : meta_constructor_traits_base<this_t, args_t...>
    {
    };
}