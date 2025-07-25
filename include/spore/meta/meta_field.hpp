#pragma once

#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_field_traits.hpp"
#include "spore/meta/meta_tuple.hpp"
#include "spore/meta/meta_string.hpp"

#include <cstddef>
#include <type_traits>

namespace spore
{
    template <std::size_t name_v, typename field_t, any_meta_attribute... attributes_t>
    struct meta_field
    {
        using this_type = typename meta_field_traits<field_t>::this_type;
        using value_type = typename meta_field_traits<field_t>::value_type;

        meta_string<name_v> name;
        field_t field;
        meta_tuple<attributes_t...> attributes;

        constexpr value_type&& get(this_type&& self) const
        {
            return std::move(self).*field;
        }

        constexpr value_type& get(this_type& self) const
        {
            return self.*field;
        }

        constexpr const value_type& get(const this_type& self) const
        {
            return self.*field;
        }
    };

    template <std::size_t name_v, typename field_t, any_meta_attribute... attributes_t>
    meta_field(const char (&)[name_v], field_t, meta_tuple<attributes_t...>)
        -> meta_field<name_v, field_t, attributes_t...>;

    template <typename>
    struct is_meta_field : std::false_type
    {
    };

    template <std::size_t name_v, typename field_t, any_meta_attribute... attributes_t>
    struct is_meta_field<meta_field<name_v, field_t, attributes_t...>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_field_v = is_meta_field<value_t>::value;

    template <typename value_t>
    concept any_meta_field = is_meta_field_v<value_t>;
}