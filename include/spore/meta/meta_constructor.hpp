#pragma once

#include "spore/meta/meta_argument.hpp"
#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_function_utils.hpp"
#include "spore/meta/meta_tuple.hpp"

#include <type_traits>

namespace spore
{
    template <typename constructor_t, typename this_t, any_meta_tuple arguments_t, any_meta_tuple attributes_t>
    struct meta_constructor;

    template <typename constructor_t, typename this_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    struct meta_constructor<constructor_t, this_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>
    {
        constructor_t constructor;
        meta_type_ref<this_t> this_type;
        meta_tuple<arguments_t...> arguments;
        meta_tuple<attributes_t...> attributes;

        template <typename... args_t>
        constexpr this_t construct(args_t&&... args) const
        {
            using actual_this_t = decltype(constructor.operator()(std::forward<args_t>(args)...));
            meta::functions::check_assignable<std::tuple<args_t&&...>, std::tuple<typename arguments_t::value_type&&...>>();
            meta::functions::check_assignable<std::tuple<actual_this_t>, std::tuple<this_t>>();
            return constructor.operator()(std::forward<args_t>(args)...);
        }

        template <typename param_t, typename... params_t, typename... args_t>
        constexpr this_t construct(args_t&&... args) const
        {
            using actual_this_t = decltype(constructor.template operator()<param_t, params_t...>(std::forward<args_t>(args)...));
            meta::functions::check_assignable<std::tuple<args_t&&...>, std::tuple<typename arguments_t::value_type&&...>>();
            meta::functions::check_assignable<std::tuple<actual_this_t>, std::tuple<this_t>>();
            return constructor.template operator()<param_t, params_t...>(std::forward<args_t>(args)...);
        }

        template <auto param_v, auto... params_v, typename... args_t>
        constexpr this_t construct(args_t&&... args) const
        {
            using actual_this_t = decltype(constructor.template operator()<param_v, params_v...>(std::forward<args_t>(args)...));
            meta::functions::check_assignable<std::tuple<args_t&&...>, std::tuple<typename arguments_t::value_type&&...>>();
            meta::functions::check_assignable<std::tuple<actual_this_t>, std::tuple<this_t>>();
            return constructor.template operator()<param_v, params_v...>(std::forward<args_t>(args)...);
        }
    };

    template <typename constructor_t, typename this_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    meta_constructor(constructor_t, meta_type_ref<this_t>, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>)
        -> meta_constructor<constructor_t, this_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>;

    template <typename>
    struct is_meta_constructor : std::false_type
    {
    };

    template <typename constructor_t, typename this_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    struct is_meta_constructor<meta_constructor<constructor_t, this_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_constructor_v = is_meta_constructor<value_t>::value;

    template <typename value_t>
    concept any_meta_constructor = is_meta_constructor_v<value_t>;
}