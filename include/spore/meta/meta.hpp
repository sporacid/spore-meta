#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_enabled.hpp"
#include "spore/meta/meta_enum.hpp"
#include "spore/meta/meta_type.hpp"

#include <concepts>
#include <string_view>

namespace spore
{
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
        { object_v.name } -> meta_string_convertible;
    };
    // clang-format on

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
        consteval any_meta_string auto get_name()
        {
            constexpr meta_type type = get_type<value_t>();
            return type.name;
        }

        template <auto object_v>
        consteval any_meta_string auto get_name() requires has_meta_name<object_v>
        {
            return object_v.name;
        }

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_base(func_t&& func)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::for_each<type.bases>(std::forward<func_t>(func));
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

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_constructor(func_t&& func)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::for_each<type.constructors>(std::forward<func_t>(func));
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

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_base(const value_t&, func_t&& func)
        {
            return for_each_base<value_t>(std::forward<func_t>(func));
        }

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_field(const value_t&, func_t&& func)
        {
            return for_each_field<value_t>(std::forward<func_t>(func));
        }

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_function(const value_t&, func_t&& func)
        {
            return for_each_function<value_t>(std::forward<func_t>(func));
        }

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_constructor(const value_t&, func_t&& func)
        {
            return for_each_constructor<value_t>(std::forward<func_t>(func));
        }

        template <meta_enum_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_value(const value_t&, func_t&& func)
        {
            return for_each_value<value_t>(std::forward<func_t>(func));
        }

        template <meta_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_attribute(const value_t&, func_t&& func)
        {
            return for_each_attribute<value_t>(std::forward<func_t>(func));
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
        constexpr auto find_base(predicate_t&& predicate)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::find<type.bases>(std::forward<predicate_t>(predicate));
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

        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_base(const value_t&, predicate_t&& predicate)
        {
            return find_base<value_t>(std::forward<predicate_t>(predicate));
        }

        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_field(const value_t&, predicate_t&& predicate)
        {
            return find_field<value_t>(std::forward<predicate_t>(predicate));
        }

        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_function(const value_t&, predicate_t&& predicate)
        {
            return find_function<value_t>(std::forward<predicate_t>(predicate));
        }

        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_constructor(const value_t&, predicate_t&& predicate)
        {
            return find_constructor<value_t>(std::forward<predicate_t>(predicate));
        }

        template <meta_enum_enabled value_t, typename predicate_t>
        constexpr auto find_value(const value_t&, predicate_t&& predicate)
        {
            return find_value<value_t>(std::forward<predicate_t>(predicate));
        }

        template <meta_enabled value_t, typename predicate_t>
        constexpr auto find_attribute(const value_t&, predicate_t&& predicate)
        {
            return find_attribute<value_t>(std::forward<predicate_t>(predicate));
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