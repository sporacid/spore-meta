#pragma once

#include "spore/meta/meta_enabled.hpp"

namespace spore::meta
{
    struct meta_init {};

    template <meta_type_enabled value_t, typename tag_t>
    bool init_type_impl(meta_adl<value_t>, meta_adl<tag_t>)
    {
        // default behavior, specialize in your library
        return true;
    }

    template <meta_enum_enabled value_t, typename tag_t>
    bool init_enum_impl(meta_adl<value_t>, meta_adl<tag_t>)
    {
        // default behavior, specialize in your library
        return true;
    }

    template <meta_type_enabled value_t>
    bool init_type()
    {
        return init_type_impl(meta_adl<value_t> {}, meta_adl<meta_init> {});
    }

    template <meta_enum_enabled value_t>
    bool init_enum()
    {
        return init_enum_impl(meta_adl<value_t> {}, meta_adl<meta_init> {});
    }

    template <meta_type_enabled value_t>
    bool is_type_init = init_type<value_t>();

    template <meta_enum_enabled value_t>
    bool is_enum_init = init_enum<value_t>();
}