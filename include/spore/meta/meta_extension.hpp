#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_enabled.hpp"

#include <concepts>

namespace spore
{
    // clang-format off
    template <typename value_t>
    concept any_meta_extension = requires(value_t value)
    {
        { []() consteval { return value_t{}; }() };
    };

    template <typename value_t>
    concept any_meta_extension_tuple = any_meta_tuple<value_t>
        and meta::tuples::all_of<value_t {}>(
            []<auto value_v> { return any_meta_extension<decltype(value_v)>; }
        );
    // clang-format on

    template <typename value_t, typename void_t>
    consteval any_meta_extension_tuple auto make_extensions(const meta_adl<value_t>, const meta_adl<void_t>)
    {
        return meta_tuple<> {};
    }

    template <typename value_t>
    consteval any_meta_extension_tuple auto make_extensions(const meta_adl<value_t>)
    {
        return make_extensions(meta_adl<value_t> {}, meta_adl<void> {});
    }
}