#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_enabled.hpp"
#include "spore/meta/meta_enum.hpp"
#include "spore/meta/meta_type.hpp"

#include <concepts>
#include <string_view>

namespace spore
{
    template <typename value_t>
    concept meta_tuple_convertible = is_meta_tuple_v<std::decay_t<value_t>>;

    // clang-format off
    template <auto object_v>
    concept has_meta_attributes = requires
    {
        { object_v.attributes } -> meta_tuple_convertible;
    };

    template <auto object_v>
    concept has_meta_arguments = requires
    {
        { object_v.arguments } -> meta_tuple_convertible;
    };

    template <auto object_v>
    concept has_meta_name = requires
    {
        { object_v.name } -> std::convertible_to<std::string_view>;
    };
    // clang-format on

    template <typename value_t, typename func_t>
    constexpr auto with_meta_type(meta_adl<value_t>, func_t&& func) = delete;

    template <typename value_t, typename func_t>
    constexpr auto with_meta_enum(meta_adl<value_t>, func_t&& func) = delete;

    namespace meta
    {
        template <meta_type_enabled value_t>
        consteval any_meta_type auto get_type()
        {
            constexpr meta_adl<value_t> adl;
            constexpr auto func = []<meta_type type_v> { return type_v; };
            return with_meta_type(adl, func);
        }

        template <meta_enum_enabled value_t>
        consteval any_meta_enum auto get_enum()
        {
            constexpr meta_adl<value_t> adl;
            constexpr auto func = []<meta_enum enum_v> { return enum_v; };
            return with_meta_enum(adl, func);
        }

        template <meta_enabled value_t>
        consteval std::string_view get_name()
        {
            constexpr meta_type type = get_type<value_t>();
            return type.name;
        }

        template <auto object_v>
        consteval std::string_view get_name() requires has_meta_name<object_v>
        {
            return object_v.name;
        }

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_field(func_t&& func)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::for_each<type.fields>(std::forward<func_t>(func));
        }

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_function(func_t&& func)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::for_each<type.functions>(std::forward<func_t>(func));
        }

        template <meta_enum_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_value(func_t&& func)
        {
            constexpr meta_enum enum_ = get_enum<value_t>();
            return tuples::for_each<enum_.values>(std::forward<func_t>(func));
        }

        template <meta_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_attribute(func_t&& func)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::for_each<type.attributes>(std::forward<func_t>(func));
        }

        template <auto object_v, typename func_t>
        constexpr meta_result auto for_each_attribute(func_t&& func) requires has_meta_attributes<object_v>
        {
            return tuples::for_each<object_v.attributes>(std::forward<func_t>(func));
        }

        template <auto object_v, typename func_t>
        constexpr meta_result auto for_each_argument(func_t&& func) requires has_meta_arguments<object_v>
        {
            return tuples::for_each<object_v.arguments>(std::forward<func_t>(func));
        }

        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_field(predicate_t&& predicate)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::find<type.fields>(std::forward<predicate_t>(predicate));
        }

        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_function(predicate_t&& predicate)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::find<type.functions>(std::forward<predicate_t>(predicate));
        }

        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_constructor(predicate_t&& predicate)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::find<type.constructors>(std::forward<predicate_t>(predicate));
        }

        template <meta_enum_enabled value_t, typename predicate_t>
        constexpr auto find_value(predicate_t&& predicate)
        {
            constexpr meta_enum enum_ = get_enum<value_t>();
            return tuples::find<enum_.values>(std::forward<predicate_t>(predicate));
        }

        template <meta_enabled value_t, typename predicate_t>
        constexpr auto find_attribute(predicate_t&& predicate)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::find<type.attributes>(std::forward<predicate_t>(predicate));
        }

        template <auto object_v, typename predicate_t>
        constexpr auto find_attribute(predicate_t&& predicate) requires has_meta_attributes<object_v>
        {
            return tuples::find<object_v.attributes>(std::forward<predicate_t>(predicate));
        }

        template <auto object_v, typename predicate_t>
        constexpr auto find_argument(predicate_t&& predicate) requires has_meta_arguments<object_v>
        {
            return tuples::find<object_v.arguments>(std::forward<predicate_t>(predicate));
        }
    }
}