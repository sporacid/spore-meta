#pragma once

#include "spore/meta/meta_argument.hpp"
#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_parameter.hpp"
#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_tuple.hpp"

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace spore
{
    template <
        std::size_t name_v,
        typename function_t,
        typename return_t,
        any_meta_tuple arguments_t,
        any_meta_tuple parameters_t,
        any_meta_tuple attributes_t>
    struct meta_function;

    template <
        std::size_t name_v,
        typename function_t,
        typename return_t,
        any_meta_argument... arguments_t,
        any_meta_parameter... parameters_t,
        any_meta_attribute... attributes_t>
    struct meta_function<
        name_v,
        function_t,
        return_t,
        meta_tuple<arguments_t...>,
        meta_tuple<parameters_t...>,
        meta_tuple<attributes_t...>>
    {
        meta_string<name_v> name;
        function_t function;
        meta_type_ref<return_t> return_type;
        meta_tuple<arguments_t...> arguments;
        meta_tuple<parameters_t...> parameters;
        meta_tuple<attributes_t...> attributes;

        static consteval bool is_template()
        {
            return sizeof...(parameters_t) > 0;
        }

        template <typename... args_t>
        static consteval bool is_invokable_with(meta_type_ref<args_t>...)
        {
            // clang-format off
            return requires { { std::declval<function_t>().operator()(std::declval<args_t>()...) } -> std::same_as<return_t>; };
            // clang-format on
        }

        template <typename param_t, typename... params_t, typename... args_t>
        static consteval bool is_invokable_with(meta_type_ref<args_t>...)
        {
            if constexpr (any_meta_parameter_ref<return_t>)
            {
                // clang-format off
                return requires { std::declval<function_t>().template operator()<param_t, params_t...>(std::declval<args_t>()...); };
                // clang-format on
            }
            else
            {
                // clang-format off
                return requires { { std::declval<function_t>().template operator()<param_t, params_t...>(std::declval<args_t>()...) } -> std::same_as<return_t>; };
                // clang-format on
            }
        }

        template <auto param_v, auto... params_v, typename... args_t>
        static consteval bool is_invokable_with(meta_type_ref<args_t>...)
        {
            if constexpr (any_meta_parameter_ref<return_t>)
            {
                // clang-format off
                return requires { std::declval<function_t>().template operator()<param_v, params_v...>(std::declval<args_t>()...); };
                // clang-format on
            }
            else
            {
                // clang-format off
                return requires { { std::declval<function_t>().template operator()<param_v, params_v...>(std::declval<args_t>()...) } -> std::same_as<return_t>; };
                // clang-format on
            }
        }

        template <typename... args_t>
        constexpr auto invoke(args_t&&... args) const
        {
            static_assert(is_invokable_with(meta_type_ref<args_t&&> {}...));
            return function.operator()(std::forward<args_t>(args)...);
        }

        template <typename param_t, typename... params_t, typename... args_t>
        constexpr auto invoke(args_t&&... args) const
        {
            static_assert(is_invokable_with<param_t, params_t...>(meta_type_ref<args_t&&> {}...));
            return function.template operator()<param_t, params_t...>(std::forward<args_t>(args)...);
        }

        template <auto param_v, auto... params_v, typename... args_t>
        constexpr auto invoke(args_t&&... args) const
        {
            static_assert(is_invokable_with<param_v, params_v...>(meta_type_ref<args_t&&> {}...));
            return function.template operator()<param_v, params_v...>(std::forward<args_t>(args)...);
        }
    };

    template <
        std::size_t name_v,
        typename function_t,
        typename return_t,
        any_meta_argument... arguments_t,
        any_meta_parameter... parameters_t,
        any_meta_attribute... attributes_t>
    meta_function(
        meta_string<name_v>, function_t,
        meta_type_ref<return_t>,
        meta_tuple<arguments_t...>,
        meta_tuple<parameters_t...>,
        meta_tuple<attributes_t...>)
        -> meta_function<
            name_v,
            function_t,
            return_t,
            meta_tuple<arguments_t...>,
            meta_tuple<parameters_t...>,
            meta_tuple<attributes_t...>>;

    template <typename>
    struct is_meta_function : std::false_type
    {
    };

    template <
        std::size_t name_v,
        typename function_t,
        typename return_t,
        any_meta_argument... arguments_t,
        any_meta_parameter... parameters_t,
        any_meta_attribute... attributes_t>
    struct is_meta_function<
        meta_function<
            name_v,
            function_t,
            return_t,
            meta_tuple<arguments_t...>,
            meta_tuple<parameters_t...>,
            meta_tuple<attributes_t...>>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_function_v = is_meta_function<value_t>::value;

    template <typename value_t>
    concept any_meta_function = is_meta_function_v<value_t>;
}