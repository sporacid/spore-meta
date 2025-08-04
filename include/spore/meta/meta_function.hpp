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
        static_assert(std::is_invocable_v<function_t, typename arguments_t::value_type&&...>, "bad function arguments");

        meta_string<name_v> name;
        function_t function;
        meta_type_ref<return_t> return_type;
        meta_tuple<arguments_t...> arguments;
        meta_tuple<attributes_t...> attributes;

        constexpr void invoke(return_t& return_, typename arguments_t::value_type&&... args) const
        {
            return_ = function(std::forward<typename arguments_t::value_type>(args)...);
        }

        constexpr return_t invoke(typename arguments_t::value_type&&... args) const
        {
            return function(std::forward<typename arguments_t::value_type>(args)...);
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