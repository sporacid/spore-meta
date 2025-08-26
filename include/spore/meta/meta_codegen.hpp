#pragma once

#ifndef SPORE_CODEGEN
#    error "This file can only be included within the codegen scope."
#endif

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_enum.hpp"
#include "spore/meta/meta_type.hpp"

#include <type_traits>

namespace spore::meta
{
    template <typename value_t>
    constexpr any_meta_type auto get_meta_type(meta_adl<value_t>)
    {
        constexpr meta_type type {
            .name = meta_string {""},
            .bases = meta_tuple {},
            .fields = meta_tuple {},
            .functions = meta_tuple {},
            .constructors = meta_tuple {},
            .attributes = meta_tuple {},
            .extensions = make_extensions(meta_adl<value_t> {}),
        };

        return type;
    }

    template <typename value_t>
    constexpr any_meta_enum auto get_meta_enum(meta_adl<value_t>)
    {
        constexpr meta_enum enum_ {
            .name = meta_string {""},
            .type = meta_type_ref<std::underlying_type_t<value_t>> {},
            .values = meta_tuple {},
            .attributes = meta_tuple {},
            .extensions = make_extensions(meta_adl<value_t> {}),
        };

        return enum_;
    }
}