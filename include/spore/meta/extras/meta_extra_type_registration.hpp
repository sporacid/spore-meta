#pragma once

#include "spore/meta/meta_adl.hpp"

namespace spore::meta
{
    template <typename value_t, typename void_t>
    void register_type(const meta_adl<value_t>, const meta_adl<void_t>)
    {
    }

    template <typename value_t>
    void register_type(const meta_adl<value_t>)
    {
        register_type(meta_adl<value_t> {}, meta_adl<void> {});
    }

    template <typename value_t>
    const bool is_type_registered = [] {
        register_type(meta_adl<value_t> {});
        return true;
    }();
}