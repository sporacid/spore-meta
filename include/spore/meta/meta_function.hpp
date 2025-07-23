#pragma once

#include "spore/meta/meta_argument.hpp"
#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_function_traits.hpp"
#include "spore/meta/meta_tuple.hpp"
#include "spore/meta/meta_utils.hpp"

#include <cstddef>
#include <type_traits>

namespace spore
{
    template <std::size_t name_v, typename...>
    struct meta_function;

    template <std::size_t name_v, typename function_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    struct meta_function<name_v, function_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>
    {
        using return_type = typename meta_function_traits<function_t>::return_type;
        using args_type = typename meta_function_traits<function_t>::args_type;

        const char name[name_v];
        function_t function;
        meta_tuple<arguments_t...> arguments;
        meta_tuple<attributes_t...> attributes;

        template <typename... args_t>
        constexpr return_type invoke(args_t&&... args) const
        {
            meta::utils::check_is_assignable<std::tuple<args_t&&...>, args_type>();
            return std::apply(function, std::tie(std::forward<args_t>(args)...));
        }
    };

    template <std::size_t name_v, typename function_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    meta_function(const char (&)[name_v], function_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>)
        -> meta_function<name_v, function_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>;

    template <typename>
    struct is_meta_function : std::false_type
    {
    };

    template <std::size_t name_v, typename function_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    struct is_meta_function<meta_function<name_v, function_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_function_v = is_meta_function<value_t>::value;

    template <typename value_t>
    concept any_meta_function = is_meta_function_v<value_t>;
}