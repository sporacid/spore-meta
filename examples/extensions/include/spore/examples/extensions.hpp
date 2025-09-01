#pragma once

#include "spore/meta.hpp"

#include <iostream>

#include SPORE_META_GENERATED("spore/examples/extensions.meta.hpp")

namespace spore::examples::extensions
{
    struct SPORE_META_TYPE(value) value
    {
    };

    struct SPORE_META_TYPE(widget) widget
    {
    };

    inline std::size_t hash_value(const value&)
    {
        std::cout << "Hash value called" << std::endl;
        return 0;
    }

    inline void draw_widget(widget&)
    {
        std::cout << "Draw widget called" << std::endl;
    }

    template <typename value_t>
    struct value_extension
    {
        using hash_value_t = std::size_t (*)(const value_t&);
        hash_value_t hash_value = nullptr;
    };

    template <typename value_t>
    struct widget_extension
    {
        using draw_widget_t = void (*)(value_t&);
        draw_widget_t draw_widget = nullptr;
    };

    template <typename value_t>
    consteval value_extension<value_t> make_value_extension()
    {
        return value_extension<value_t> {
            .hash_value = [](const value_t& value) { return hash_value(value); },
        };
    }

    template <typename value_t>
    consteval widget_extension<value_t> make_widget_extension()
    {
        return widget_extension<value_t> {
            .draw_widget = [](value_t& value) { draw_widget(value); },
        };
    }

    // clang-format off
    template <typename value_t>
    concept any_value = meta_enabled<value_t> and requires(const value_t& value)
    {
        { hash_value(value) } -> std::convertible_to<std::size_t>;
    };

    template <typename value_t>
    concept any_widget = meta_enabled<value_t> and requires(value_t& value)
    {
        { draw_widget(value) } -> std::same_as<void>;
    };
    // clang-format on

    template <any_value value_t>
    consteval any_meta_tuple_of<is_meta_extension> auto get_extensions(const meta_adl<value_t>)
    {
        return meta_tuple {
            make_value_extension<value_t>(),
        };
    }

    template <any_widget value_t>
    consteval any_meta_tuple_of<is_meta_extension> auto get_extensions(const meta_adl<value_t>)
    {
        return meta_tuple {
            make_widget_extension<value_t>(),
        };
    }
}

#include SPORE_META_GENERATED("spore/examples/extensions.meta.inl")