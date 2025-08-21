#pragma once

#include "spore/meta/meta_argument.hpp"
#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_tuple.hpp"

#include <cstddef>
#include <type_traits>

namespace spore
{
    template <std::size_t name_v, typename function_t, typename return_t, any_meta_tuple arguments_t, any_meta_tuple attributes_t>
    struct meta_function;

    template <std::size_t name_v, typename function_t, typename return_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    struct meta_function<name_v, function_t, return_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>
    {
        meta_string<name_v> name;
        function_t function;
        meta_type_ref<return_t> return_type;
        meta_tuple<arguments_t...> arguments;
        meta_tuple<attributes_t...> attributes;

        template <typename... args_t>
        constexpr auto invoke(args_t&&... args) const
        {
            using actual_return_t = decltype(function.operator()(std::forward<args_t>(args)...));
            meta::functions::check_assignable<std::tuple<args_t&&...>, std::tuple<typename arguments_t::value_type&&...>>();
            meta::functions::check_assignable<std::tuple<actual_return_t>, std::tuple<return_t>>();
            return function.operator()(std::forward<args_t>(args)...);
        }

        template <typename param_t, typename... params_t, typename... args_t>
        constexpr auto invoke(args_t&&... args) const
        {
            using actual_return_t = decltype(function.template operator()<param_t, params_t...>(std::forward<args_t>(args)...));
            meta::functions::check_assignable<std::tuple<args_t&&...>, std::tuple<typename arguments_t::value_type&&...>>();
            meta::functions::check_assignable<std::tuple<actual_return_t>, std::tuple<return_t>>();
            return function.template operator()<param_t, params_t...>(std::forward<args_t>(args)...);
        }

        template <auto param_v, auto... params_v, typename... args_t>
        constexpr auto invoke(args_t&&... args) const
        {
            using actual_return_t = decltype(function.template operator()<param_v, params_v...>(std::forward<args_t>(args)...));
            meta::functions::check_assignable<std::tuple<args_t&&...>, std::tuple<typename arguments_t::value_type&&...>>();
            meta::functions::check_assignable<std::tuple<actual_return_t>, std::tuple<return_t>>();
            return function.template operator()<param_v, params_v...>(std::forward<args_t>(args)...);
        }
    };

    template <std::size_t name_v, typename function_t, typename return_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    meta_function(meta_string<name_v>, function_t, meta_type_ref<return_t>, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>)
        -> meta_function<name_v, function_t, return_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>;

    template <typename>
    struct is_meta_function : std::false_type
    {
    };

    template <std::size_t name_v, typename function_t, typename return_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    struct is_meta_function<meta_function<name_v, function_t, return_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_function_v = is_meta_function<value_t>::value;

    template <typename value_t>
    concept any_meta_function = is_meta_function_v<value_t>;
}