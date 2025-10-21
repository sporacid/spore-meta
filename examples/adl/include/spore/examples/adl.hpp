#pragma once

#include "spore/meta/meta.hpp"

#include <iostream>

#include SPORE_META_GENERATED("spore/examples/adl.meta.hpp")

namespace ns1
{
    struct SPORE_META_TYPE() t1
    {
    };

    template <typename value_t>
    void register_type(const spore::meta_adl<value_t>)
    {
        std::cout << "type registered from ns1" << std::endl;
    }
}

namespace ns2
{
    struct SPORE_META_TYPE() t2
    {
    };

    template <typename value_t>
    void register_type(const spore::meta_adl<value_t>)
    {
        std::cout << "type registered from ns2" << std::endl;
    }

    namespace ns3
    {
        template <typename value_t>
        struct SPORE_META_TYPE() t3
        {
        };

        using ns2::register_type;
    }
}

#include SPORE_META_GENERATED("spore/examples/adl.meta.inl")