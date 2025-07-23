#pragma once

#include <string>

#include "spore/meta.hpp"

#include SPORE_META_GENERATED("spore/examples/hello_world.meta.hpp")

namespace spore::examples::hello_world
{
    struct SPORE_META_TYPE() hello_world_fields
    {
        SPORE_META_FIELD()
        std::string message = "Hello world";

        SPORE_META_FIELD()
        std::string from = "sporacid";
    };

    struct SPORE_META_TYPE() hello_world_functions
    {
        SPORE_META_FUNCTION()
        std::string get_message() const
        {
            return "Hello world";
        }

        SPORE_META_FUNCTION()
        std::string get_from() const
        {
            return "sporacid";
        }
    };

    struct SPORE_META_TYPE(message = "Hello world", from = "sporacid") hello_world_attributes
    {
    };

    enum class SPORE_META_ENUM() hello_world_enum
    {
        hello_world SPORE_META_ENUM_VALUE(display = "Hello world"),
        sporacid    SPORE_META_ENUM_VALUE(),
    };
}

#include SPORE_META_GENERATED("spore/examples/hello_world.meta.inl")