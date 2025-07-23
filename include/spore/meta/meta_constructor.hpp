#pragma once

#include "spore/meta/meta_argument.hpp"
#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_constructor_traits.hpp"
#include "spore/meta/meta_tuple.hpp"
#include "spore/meta/meta_utils.hpp"

#include <type_traits>

namespace spore
{
    template <typename...>
    struct meta_constructor;

    template <typename constructor_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    struct meta_constructor<constructor_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>
    {
        using this_type = typename meta_constructor_traits<constructor_t>::this_type;
        using args_type = typename meta_constructor_traits<constructor_t>::args_type;

        constructor_t constructor;
        meta_tuple<arguments_t...> arguments;
        meta_tuple<attributes_t...> attributes;

        template <typename... args_t>
        constexpr void construct(this_type& self, args_t&&... args) const
        {
            meta::utils::check_is_assignable<std::tuple<args_t&&...>, args_type>();
            std::apply(constructor, std::tie(self, std::forward<args_t>(args)...));
        }
    };

    template <typename constructor_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    meta_constructor(constructor_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>)
        -> meta_constructor<constructor_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>;

    template <typename>
    struct is_meta_constructor : std::false_type
    {
    };

    template <typename constructor_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    struct is_meta_constructor<meta_constructor<constructor_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_constructor_v = is_meta_constructor<value_t>::value;

    template <typename value_t>
    concept any_meta_constructor = is_meta_constructor_v<value_t>;
}