#pragma once

#include "spore/meta.hpp"

#include <iostream>

#include SPORE_META_GENERATED("spore/examples/extensions.meta.hpp")

namespace spore::examples::extensions
{
    struct SPORE_META_TYPE(extensions = (extension)) widget
    {
    };

    inline void draw_widget(widget& value);

    struct extension
    {
        using draw_widget_t = void (*)(void*);
        draw_widget_t draw_widget = nullptr;
    };

    template <typename value_t>
    consteval extension make_extension(meta_adl<extension>, meta_adl<value_t>)
    {
        extension extension;

        // clang-format off
        constexpr bool has_draw_widget = requires
        {
            { draw_widget(std::declval<value_t&>()) } -> std::same_as<void>;
        };
        // clang-format on

        if constexpr (has_draw_widget)
        {
            extension.draw_widget = [](void* widget) { draw_widget(*static_cast<value_t*>(widget)); };
        }

        return extension;
    }
}

#include SPORE_META_GENERATED("spore/examples/extensions.meta.inl")

void spore::examples::extensions::draw_widget(widget& value)
{
    std::cout << "Draw widget called with " << meta::get_name<widget>() << std::endl;
}