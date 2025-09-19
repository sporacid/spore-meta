#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_enabled.hpp"
#include "spore/meta/meta_enum.hpp"
#include "spore/meta/meta_extension.hpp"
#include "spore/meta/meta_type.hpp"
#include "spore/meta/meta_type_id.hpp"

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
        namespace detail
        {
            struct default_type
            {
                int default_field;
            };

            struct default_extension
            {
            };

            constexpr meta_base default_base {
                .type = meta_type_ref<default_type> {},
            };

            constexpr meta_field default_field {
                .name = meta_string {""},
                .field = &default_type::default_field,
                .attributes = meta_tuple {},
            };

            constexpr meta_constructor default_constructor {
                .constructor = [](default_type&) { return default_type {}; },
                .this_type = meta_type_ref<default_type> {},
                .arguments = meta_tuple {},
                .parameters = meta_tuple {},
                .attributes = meta_tuple {},
            };

            constexpr meta_function default_function {
                .name = meta_string {""},
                .function = [] {},
                .return_type = meta_type_ref<void> {},
                .arguments = meta_tuple {},
                .parameters = meta_tuple {},
                .attributes = meta_tuple {},
            };

            constexpr meta_enum_value default_enum_value {
                .name = meta_string {""},
                .value = 0,
                .attributes = meta_tuple {},
            };

            constexpr meta_attribute default_attribute {
                .name = meta_string {""},
                .value = false,
            };
        }

        template <meta_type_enabled value_t>
        constexpr any_meta_type auto get_type()
        {
            return get_meta_type(meta_adl<value_t> {});
        }

        template <meta_enum_enabled value_t>
        constexpr any_meta_enum auto get_enum()
        {
            return get_meta_enum(meta_adl<value_t> {});
        }

        template <meta_type_enabled value_t>
        constexpr meta_type_id get_id()
        {
            constexpr meta_type_id type_id {get_type<value_t>()};
            return type_id;
        }

        template <meta_enum_enabled value_t>
        constexpr meta_type_id get_id()
        {
            constexpr meta_type_id type_id {get_enum<value_t>()};
            return type_id;
        }

        template <meta_type_enabled value_t>
        constexpr any_meta_string auto get_name()
        {
            constexpr meta_type type = get_type<value_t>();
            return type.name;
        }

        template <meta_enum_enabled value_t>
        constexpr any_meta_string auto get_name()
        {
            constexpr meta_enum enum_ = get_enum<value_t>();
            return enum_.name;
        }

        template <auto object_v>
        constexpr any_meta_string auto get_name() requires has_meta_name<object_v>
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
        constexpr meta_result auto for_each_base_r(func_t&& func)
        {
            if constexpr (utils::is_constexpr_invocable<std::decay_t<func_t>, detail::default_base>())
            {
                constexpr auto wrapper = [=]<meta_base base_v> {
                    using base_t = typename decltype(base_v)::value_type;

                    constexpr meta_result auto result = for_each_base_r<base_t>(func);

                    if constexpr (is_meta_continue_v<decltype(result)>)
                    {
                        return func.template operator()<base_v>();
                    }
                    else
                    {
                        return result;
                    }
                };

                return for_each_base<value_t>(wrapper);
            }
            else
            {
                const auto wrapper = [&]<meta_base base_v> {
                    using base_t = typename decltype(base_v)::value_type;

                    meta_result auto result = for_each_base_r<base_t>(func);

                    if constexpr (is_meta_continue_v<decltype(result)>)
                    {
                        return func.template operator()<base_v>();
                    }
                    else
                    {
                        return result;
                    }
                };

                return for_each_base<value_t>(wrapper);
            }
        }

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_field(func_t&& func)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::for_each<type.fields>(std::forward<func_t>(func));
        }

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_field_r(func_t&& func)
        {
            if constexpr (utils::is_constexpr_invocable<std::decay_t<func_t>, detail::default_field>())
            {
                constexpr auto wrapper = [=]<meta_base base_v> {
                    constexpr auto field_wrapper = [=]<meta_field field_v> {
                        constexpr auto predicate = [=]<meta_field super_field_v> { return super_field_v.name.get() == field_v.name; };
                        constexpr auto super_field = find_field<value_t>(predicate);

                        if constexpr (meta::is_invalid(super_field))
                        {
                            return func.template operator()<field_v>();
                        }
                    };

                    using base_t = typename decltype(base_v)::value_type;
                    return for_each_field<base_t>(field_wrapper);
                };

                constexpr meta_result auto result = for_each_base_r<value_t>(wrapper);

                if constexpr (is_meta_continue_v<decltype(result)>)
                {
                    return for_each_field<value_t>(func);
                }
                else
                {
                    return result;
                }
            }
            else
            {
                const auto wrapper = [&]<meta_base base_v> {
                    const auto field_wrapper = [&]<meta_field field_v> {
                        constexpr auto predicate = [=]<meta_field super_field_v> { return super_field_v.name.get() == field_v.name; };
                        constexpr auto super_field = find_field<value_t>(predicate);

                        if constexpr (meta::is_invalid(super_field))
                        {
                            return func.template operator()<field_v>();
                        }
                    };

                    using base_t = typename decltype(base_v)::value_type;
                    return for_each_field<base_t>(field_wrapper);
                };

                meta_result auto result = for_each_base_r<value_t>(wrapper);

                if constexpr (is_meta_continue_v<decltype(result)>)
                {
                    return for_each_field<value_t>(func);
                }
                else
                {
                    return result;
                }
            }
        }

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_function(func_t&& func)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::for_each<type.functions>(std::forward<func_t>(func));
        }

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_function_r(func_t&& func)
        {
            if constexpr (utils::is_constexpr_invocable<std::decay_t<func_t>, detail::default_function>())
            {
                constexpr auto wrapper = [=]<meta_base base_v> {
                    constexpr auto function_wrapper = [&]<meta_function function_v> {
                        constexpr auto predicate = [=]<meta_function super_function_v> { return super_function_v.name.get() == function_v.name; };
                        constexpr auto super_function = find_function<value_t>(predicate);

                        if constexpr (meta::is_invalid(super_function))
                        {
                            return func.template operator()<function_v>();
                        }
                    };

                    using base_t = typename decltype(base_v)::value_type;
                    return for_each_function<base_t>(function_wrapper);
                };

                constexpr meta_result auto result = for_each_base_r<value_t>(wrapper);

                if constexpr (is_meta_continue_v<decltype(result)>)
                {
                    return for_each_function<value_t>(func);
                }
                else
                {
                    return result;
                }
            }
            else
            {
                const auto wrapper = [&]<meta_base base_v> {
                    const auto function_wrapper = [&]<meta_function function_v> {
                        constexpr auto predicate = [=]<meta_function super_function_v> { return super_function_v.name.get() == function_v.name; };
                        constexpr auto super_function = find_function<value_t>(predicate);

                        if constexpr (meta::is_invalid(super_function))
                        {
                            return func.template operator()<function_v>();
                        }
                    };

                    using base_t = typename decltype(base_v)::value_type;
                    return for_each_function<base_t>(function_wrapper);
                };

                meta_result auto result = for_each_base_r<value_t>(wrapper);

                if constexpr (is_meta_continue_v<decltype(result)>)
                {
                    return for_each_function<value_t>(func);
                }
                else
                {
                    return result;
                }
            }
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

        template <meta_type_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_attribute(func_t&& func)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::for_each<type.attributes>(std::forward<func_t>(func));
        }

        template <meta_enum_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_attribute(func_t&& func)
        {
            constexpr meta_enum enum_ = get_enum<value_t>();
            return tuples::for_each<enum_.attributes>(std::forward<func_t>(func));
        }

        template <meta_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_extension(func_t&& func)
        {
            constexpr any_meta_tuple_of<is_meta_extension> auto extensions = get_extensions<value_t>();
            return tuples::for_each<extensions>(std::forward<func_t>(func));
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

        template <meta_enabled value_t, typename func_t>
        constexpr meta_result auto for_each_extension(const value_t&, func_t&& func)
        {
            return for_each_extension<value_t>(std::forward<func_t>(func));
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
            return tuples::find<type.bases, detail::default_base>(std::forward<predicate_t>(predicate));
        }

#if 0
        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_base_r(predicate_t&& predicate)
        {
            if constexpr (utils::is_constexpr_invocable<std::decay_t<predicate_t>, detail::default_base>())
            {
                constexpr auto wrapper = [=]<meta_base base_v> {
                    using base_t = typename decltype(base_v)::value_type;

                    constexpr auto base = find_base<base_t>(predicate);

                    if constexpr (meta::is_valid(base))
                    {
                        return base;
                    }
                    else if constexpr (predicate_t {}.template operator()<base_v>())
                    {
                        return base_v;
                    }
                    else
                    {
                        return meta_invalid {};
                    }
                };

                return find_base<value_t>(wrapper);
            }
            else
            {
                const auto wrapper = [&]<meta_base base_v> {
                    using base_t = typename decltype(base_v)::value_type;

                    const auto base = find_base_r<base_t>(predicate);

                    if (meta::is_valid(base))
                    {
                        return base;
                    }
                    else
                    {
                        if (predicate.template operator()<base_v>())
                        {
                            return base_v;
                        }
                    }
                    else
                    {
                        return meta_invalid {};
                    }
                };

                return for_each_base<value_t>(wrapper);
            }
        }
#endif

        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_field(predicate_t&& predicate)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::find<type.fields, detail::default_field>(std::forward<predicate_t>(predicate));
        }

        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_function(predicate_t&& predicate)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::find<type.functions, detail::default_function>(std::forward<predicate_t>(predicate));
        }

        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_constructor(predicate_t&& predicate)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::find<type.constructors, detail::default_constructor>(std::forward<predicate_t>(predicate));
        }

        template <meta_enum_enabled value_t, typename predicate_t>
        constexpr auto find_value(predicate_t&& predicate)
        {
            constexpr meta_enum enum_ = get_enum<value_t>();
            return tuples::find<enum_.values, detail::default_enum_value>(std::forward<predicate_t>(predicate));
        }

        template <meta_type_enabled value_t, typename predicate_t>
        constexpr auto find_attribute(predicate_t&& predicate)
        {
            constexpr meta_type type = get_type<value_t>();
            return tuples::find<type.attributes, detail::default_attribute>(std::forward<predicate_t>(predicate));
        }

        template <meta_enum_enabled value_t, typename predicate_t>
        constexpr auto find_attribute(predicate_t&& predicate)
        {
            constexpr meta_enum enum_ = get_enum<value_t>();
            return tuples::find<enum_.attributes, detail::default_attribute>(std::forward<predicate_t>(predicate));
        }

        template <meta_enabled value_t, typename predicate_t>
        constexpr auto find_extension(predicate_t&& predicate)
        {
            constexpr any_meta_tuple_of<is_meta_extension> auto extensions = get_extensions<value_t>();
            return tuples::find<extensions, detail::default_extension {}>(std::forward<predicate_t>(predicate));
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

        template <meta_enabled value_t, typename predicate_t>
        constexpr auto find_extension(const value_t&, predicate_t&& predicate)
        {
            return find_extension<value_t>(std::forward<predicate_t>(predicate));
        }

        template <auto object_v, typename predicate_t>
        constexpr auto find_attribute(predicate_t&& predicate) requires has_meta_attributes<object_v>
        {
            return tuples::find<object_v.attributes, detail::default_attribute>(std::forward<predicate_t>(predicate));
        }

        template <auto object_v, typename predicate_t>
        constexpr auto find_argument(predicate_t&& predicate) requires has_meta_arguments<object_v>
        {
            constexpr meta_argument value_hint {
                .name = meta_string {""},
                .type = meta_type_ref<int> {},
                .attributes = meta_tuple {},
            };

            return tuples::find<object_v.arguments, value_hint>(std::forward<predicate_t>(predicate));
        }
    }
}