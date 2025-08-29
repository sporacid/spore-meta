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
        any_meta_tuple_of<is_meta_argument> arguments_t,
        any_meta_tuple_of<is_meta_parameter> parameters_t,
        any_meta_tuple_of<is_meta_attribute> attributes_t>
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

        template <typename... params_t, typename... args_t>
        static consteval bool is_invokable_with(meta_type_ref<args_t>...)
        {
            if constexpr (any_meta_parameter_ref<return_t>)
            {
                if constexpr (sizeof...(params_t) > 0)
                {
                    // clang-format off
                    return requires { std::declval<function_t>().template operator()<params_t...>(std::declval<args_t>()...); };
                    // clang-format on
                }
                else
                {
                    // clang-format off
                    return requires { std::declval<function_t>().operator()(std::declval<args_t>()...); };
                    // clang-format on
                }
            }
            else
            {
                if constexpr (sizeof...(params_t) > 0)
                {
                    // clang-format off
                    return requires { { std::declval<function_t>().template operator()<params_t...>(std::declval<args_t>()...) } -> std::same_as<return_t>; };
                    // clang-format on
                }
                else
                {
                    // clang-format off
                    return requires { { std::declval<function_t>().operator()(std::declval<args_t>()...) } -> std::same_as<return_t>; };
                    // clang-format on
                }
            }
        }

        template <typename... params_t, typename... args_t>
        constexpr auto invoke(args_t&&... args) const
        {
            static_assert(is_invokable_with<params_t...>(meta_type_ref<args_t&&> {}...));

            if constexpr (sizeof...(params_t) > 0)
            {
                return function.template operator()<params_t...>(std::forward<args_t>(args)...);
            }
            else
            {
                return function.operator()(std::forward<args_t>(args)...);
            }
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
    concept any_meta_function = is_meta_function<value_t>::value;
}