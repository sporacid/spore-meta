#pragma once

#include "spore/meta.hpp"

#include <iostream>

#include SPORE_META_GENERATED("spore/examples/extensions.meta.hpp")

namespace spore::examples::extensions
{
    struct SPORE_META_TYPE(extension) widget
    {
    };

    inline void draw_widget(widget& value)
    {
        std::cout << "Draw widget called" << std::endl;
    }

    template <typename value_t>
    struct extension
    {
        using draw_widget_t = void (*)(value_t&);
        draw_widget_t draw_widget = nullptr;
    };

    template <typename value_t>
    consteval extension<value_t> make_extension()
    {
        extension<value_t> extension;

        // clang-format off
        constexpr bool has_draw_widget = requires
        {
            { draw_widget(std::declval<value_t&>()) } -> std::same_as<void>;
        };
        // clang-format on

        if constexpr (has_draw_widget)
        {
            extension.draw_widget = &draw_widget;
        }

        return extension;
    }

     template <typename value_t>
     consteval any_meta_extension_tuple auto make_extensions(const meta_adl<value_t>, const meta_adl<void>)
     {
         return meta_tuple {
             make_extension<value_t>(),
         };
     }
}

#include SPORE_META_GENERATED("spore/examples/extensions.meta.inl")