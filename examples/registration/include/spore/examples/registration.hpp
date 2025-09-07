#pragma once

#include "spore/meta/meta.hpp"

#include SPORE_META_GENERATED("spore/examples/registration.meta.hpp")

namespace spore::examples::registration
{
    template <typename value_t>
    struct SPORE_META_TYPE() type_template
    {
    };

    struct SPORE_META_TYPE() type1
    {
        SPORE_META_FIELD()
        type_template<int> value;
    };

    struct SPORE_META_TYPE() type2
    {
        SPORE_META_FIELD()
        type_template<float> value;
    };
}

#include SPORE_META_GENERATED("spore/examples/registration.meta.inl")