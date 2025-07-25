#pragma once

#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_base.hpp"
#include "spore/meta/meta_constructor.hpp"
#include "spore/meta/meta_field.hpp"
#include "spore/meta/meta_function.hpp"
#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_tuple.hpp"

#include <cstddef>
#include <type_traits>

namespace spore
{
    template <std::size_t name_v, typename...>
    struct meta_type;

    template <
        std::size_t name_v,
        any_meta_base... bases_t,
        any_meta_field... fields_t,
        any_meta_function... functions_t,
        any_meta_constructor... constructors_t,
        any_meta_attribute... attributes_t>
    struct meta_type<
        name_v,
        meta_tuple<bases_t...>,
        meta_tuple<fields_t...>,
        meta_tuple<functions_t...>,
        meta_tuple<constructors_t...>,
        meta_tuple<attributes_t...>>
    {
        meta_string<name_v> name;
        meta_tuple<bases_t...> bases;
        meta_tuple<fields_t...> fields;
        meta_tuple<functions_t...> functions;
        meta_tuple<constructors_t...> constructors;
        meta_tuple<attributes_t...> attributes;
    };

    template <std::size_t name_v>
    struct meta_type<name_v>
    {
        meta_string<name_v> name;
        meta_tuple<> bases;
        meta_tuple<> fields;
        meta_tuple<> functions;
        meta_tuple<> constructors;
        meta_tuple<> attributes;
    };

    template <
        std::size_t name_v,
        any_meta_base... bases_t,
        any_meta_field... fields_t,
        any_meta_function... functions_t,
        any_meta_constructor... constructors_t,
        any_meta_attribute... attributes_t>
    meta_type(
        const char (&)[name_v],
        meta_tuple<bases_t...>,
        meta_tuple<fields_t...>,
        meta_tuple<functions_t...>,
        meta_tuple<constructors_t...>,
        meta_tuple<attributes_t...>)
        -> meta_type<
            name_v,
            meta_tuple<bases_t...>,
            meta_tuple<fields_t...>,
            meta_tuple<functions_t...>,
            meta_tuple<constructors_t...>,
            meta_tuple<attributes_t...>>;

    template <typename>
    struct is_meta_type : std::false_type
    {
    };

    template <
        std::size_t name_v,
        any_meta_base... bases_t,
        any_meta_field... fields_t,
        any_meta_function... functions_t,
        any_meta_constructor... constructors_t,
        any_meta_attribute... attributes_t>
    struct is_meta_type<meta_type<
        name_v,
        meta_tuple<bases_t...>,
        meta_tuple<fields_t...>,
        meta_tuple<functions_t...>,
        meta_tuple<constructors_t...>,
        meta_tuple<attributes_t...>>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_type_v = is_meta_type<value_t>::value;

    template <typename value_t>
    concept any_meta_type = is_meta_type_v<value_t>;
}