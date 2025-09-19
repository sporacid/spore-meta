#pragma once

#include <string>

#include "spore/meta/meta.hpp"

#include SPORE_META_GENERATED("spore/examples/hello_world.meta.hpp")

namespace spore::examples::hello_world
{
    struct SPORE_META_TYPE() base1
    {
        SPORE_META_FIELD()
        int b11;

        SPORE_META_FIELD()
        int b12;

        SPORE_META_FUNCTION()
        void f11() {}

        SPORE_META_FUNCTION()
        void f12() {}

        SPORE_META_FUNCTION()
        virtual void f124124() {}
    };

    struct SPORE_META_TYPE() base2
    {
        SPORE_META_FIELD()
        int b21;

        SPORE_META_FIELD()
        int b22;

        SPORE_META_FUNCTION()
        void f21() {}

        SPORE_META_FUNCTION()
        void f22() {}
    };

    struct SPORE_META_TYPE() base3 : base1
    {
        SPORE_META_FIELD()
        int b31;

        SPORE_META_FUNCTION()
        void f31() {}
    };

    struct SPORE_META_TYPE() impl : base3, base2
    {
        SPORE_META_FIELD()
        int b11;

        SPORE_META_FIELD()
        int b31;

        SPORE_META_FIELD()
        int b41;

        SPORE_META_FIELD()
        int b42;

        SPORE_META_FIELD()
        int b43;

        SPORE_META_FUNCTION()
        void f124124() override {}
    };

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
        sporacid,
    };
}

#include SPORE_META_GENERATED("spore/examples/hello_world.meta.inl")