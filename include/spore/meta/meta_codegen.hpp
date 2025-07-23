#pragma once

#ifndef SPORE_CODEGEN
#    error "This file can only be included within the codegen scope."
#endif

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_enum.hpp"
#include "spore/meta/meta_type.hpp"

namespace spore::meta
{
    template <typename value_t, typename func_t>
    constexpr auto with_meta_type(meta_adl<value_t>, func_t&& func)
    {
        constexpr meta_type type {
            .name = "",
            .fields = meta_tuple {},
            .functions = meta_tuple {},
            .constructors = meta_tuple {},
            .attributes = meta_tuple {},
        };

        return func.template operator()<type>();
    }

    template <typename value_t, typename func_t>
    constexpr auto with_meta_enum(meta_adl<value_t>, func_t&& func)
    {
        constexpr meta_enum enum_ {
            .name = "",
            .values = meta_tuple {},
            .attributes = meta_tuple {},
        };

        return func.template operator()<enum_>();
    }
}