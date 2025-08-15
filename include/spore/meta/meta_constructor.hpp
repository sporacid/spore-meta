#pragma once

#include "spore/meta/meta_argument.hpp"
#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_tuple.hpp"

#include <type_traits>

namespace spore
{
    template <typename constructor_t, typename this_t, any_meta_tuple arguments_t, any_meta_tuple attributes_t>
    struct meta_constructor;

    template <typename constructor_t, typename this_t, any_meta_argument... arguments_t, any_meta_attribute... attributes_t>
    struct meta_constructor<constructor_t, this_t, meta_tuple<arguments_t...>, meta_tuple<attributes_t...>>
    {
        // static_assert(std::is_invocable_v<constructor_t, this_t&, typename arguments_t::value_type&&...>, "bad constructor arguments");

        constructor_t constructor;
        meta_type_ref<this_t> this_type;
        meta_tuple<arguments_t...> arguments;
        meta_tuple<attributes_t...> attributes;

        template <typename... params_t, typename... args_t>
        constexpr void construct(this_t& this_, args_t&&... args) const
        {
            constructor.template operator()<params_t...>(this_, std::forward<args_t>(args)...);
        }

        template <typename... params_t, typename... args_t>
        constexpr this_t construct(args_t&&... args) const
        {
            alignas(this_t) std::byte storage[sizeof(this_t)];
            constructor.template operator()<params_t...>(reinterpret_cast<this_t&>(storage), std::forward<args_t>(args)...);
            return reinterpret_cast<this_t&&>(storage);
        }

//        template <typename... params_t>
//        constexpr void construct(this_t& this_, typename arguments_t::value_type&&... args) const
//        {
//            constructor.template operator()<params_t...>(this_, std::forward<typename arguments_t::value_type>(args)...);
//        }
//
//        template <typename... params_t>
//        constexpr this_t construct(typename arguments_t::value_type&&... args) const
//        {
//            this_t this_;
//            constructor.template operator()<params_t...>(this_, std::forward<typename arguments_t::value_type>(args)...);
//            return this_;
//        }
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