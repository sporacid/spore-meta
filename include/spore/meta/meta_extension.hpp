#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_enabled.hpp"

#include <concepts>

namespace spore
{
    template <typename value_t>
    concept any_meta_extension = requires
    {
        { []<auto> {}.template operator()<value_t {}>() };
    };

    template <typename value_t>
    struct is_meta_extension : std::false_type
    {
    };

    template <any_meta_extension value_t>
    struct is_meta_extension<value_t> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_extension_v = is_meta_extension<value_t>::value;

    namespace meta
    {
        template <meta_enabled value_t>
        consteval any_meta_tuple_of<is_meta_extension> auto get_extensions(const meta_adl<value_t>)
        {
            return meta_tuple<> {};
        }

        template <meta_enabled value_t>
        consteval any_meta_tuple_of<is_meta_extension> auto get_extensions()
        {
            return get_extensions(meta_adl<value_t> {});
        }
    }
}