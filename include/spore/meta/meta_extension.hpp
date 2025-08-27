#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_enabled.hpp"

#include <concepts>

namespace spore
{
    struct meta_extension
    {
    };

    template <typename value_t>
    struct is_meta_extension : std::false_type
    {
    };

    template <std::derived_from<meta_extension> value_t>
    struct is_meta_extension<value_t> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_extension_v = is_meta_extension<value_t>::value;

    template <typename value_t>
    concept any_meta_extension = is_meta_extension_v<value_t>;

    template <typename value_t, typename void_t>
    consteval any_meta_tuple_of<is_meta_extension> auto make_extensions(const meta_adl<value_t>, const meta_adl<void_t>)
    {
        return meta_tuple<> {};
    }

    template <typename value_t>
    consteval any_meta_tuple_of<is_meta_extension> auto make_extensions(const meta_adl<value_t>)
    {
        return make_extensions(meta_adl<value_t> {}, meta_adl<void> {});
    }
}