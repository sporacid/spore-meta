#pragma once

#include "spore/meta/meta_argument.hpp"
#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_parameter.hpp"
#include "spore/meta/meta_tuple.hpp"

#include <tuple>
#include <type_traits>

namespace spore
{
    template <
        typename constructor_t,
        typename this_t,
        any_meta_tuple_of<is_meta_argument> arguments_t,
        any_meta_tuple_of<is_meta_parameter> parameters_t,
        any_meta_tuple_of<is_meta_attribute> attributes_t>
    struct meta_constructor;

    template <
        typename constructor_t,
        typename this_t,
        any_meta_argument... arguments_t,
        any_meta_parameter... parameters_t,
        any_meta_attribute... attributes_t>
    struct meta_constructor<constructor_t, this_t, meta_tuple<arguments_t...>, meta_tuple<parameters_t...>, meta_tuple<attributes_t...>>
    {
        constructor_t constructor;
        meta_type_ref<this_t> this_type;
        meta_tuple<arguments_t...> arguments;
        meta_tuple<parameters_t...> parameters;
        meta_tuple<attributes_t...> attributes;

        static constexpr bool is_template()
        {
            return sizeof...(parameters_t) > 0;
        }

        template <typename... args_t>
        static constexpr bool is_constructible_with(meta_type_ref<args_t>...)
        {
            // clang-format off
            return requires { { std::declval<constructor_t>().operator()(std::declval<args_t>()...) } -> std::same_as<this_t>; };
            // clang-format on
        }

        template <typename... args_t>
        constexpr this_t construct(args_t&&... args) const
        {
            static_assert(is_constructible_with(meta_type_ref<args_t&&> {}...));
            return constructor.operator()(std::forward<args_t>(args)...);
        }
    };

    template <
        typename constructor_t,
        typename this_t,
        any_meta_argument... arguments_t,
        any_meta_parameter... parameters_t,
        any_meta_attribute... attributes_t>
    meta_constructor(
        constructor_t,
        meta_type_ref<this_t>,
        meta_tuple<arguments_t...>,
        meta_tuple<parameters_t...>,
        meta_tuple<attributes_t...>)
        -> meta_constructor<
            constructor_t,
            this_t,
            meta_tuple<arguments_t...>,
            meta_tuple<parameters_t...>,
            meta_tuple<attributes_t...>>;

    template <typename>
    struct is_meta_constructor : std::false_type
    {
    };

    template <
        typename constructor_t,
        typename this_t,
        any_meta_argument... arguments_t,
        any_meta_parameter... parameters_t,
        any_meta_attribute... attributes_t>
    struct is_meta_constructor<
        meta_constructor<
            constructor_t,
            this_t,
            meta_tuple<arguments_t...>,
            meta_tuple<parameters_t...>,
            meta_tuple<attributes_t...>>> : std::true_type
    {
    };

    template <typename value_t>
    concept any_meta_constructor = is_meta_constructor<value_t>::value;
}