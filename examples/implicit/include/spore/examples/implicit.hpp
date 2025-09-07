#pragma once

#include "spore/meta/meta.hpp"

#include SPORE_META_GENERATED("spore/examples/implicit.meta.hpp")

namespace spore::examples::implicit
{
    struct my_struct
    {
        int i;
        float f;

        my_struct(const int i, const float f)
            : i(i), f(f) {}

        bool valid() const
        {
            return true;
        }
    };

    enum class my_enum
    {
        value0,
        value1,
    };
}

#include SPORE_META_GENERATED("spore/examples/implicit.meta.inl")