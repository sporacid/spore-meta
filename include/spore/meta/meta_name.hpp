#pragma once

#include "spore/meta/meta_enabled.hpp"
#include "spore/meta/meta_string.hpp"

namespace spore::meta::utils
{
    namespace detail
    {
        template <typename value_t>
        concept cv_qualified = std::is_const_v<value_t> or std::is_volatile_v<value_t>;

        template <meta_enabled value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t>);

        template <cv_qualified value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t>);

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t&>);

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t&&>);

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t*>);

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t[]>);

        template <typename value_t, std::size_t size_v>
        consteval any_meta_string auto to_string_impl(meta_type_ref<value_t[size_v]>);

        consteval any_meta_string auto to_string_impl(const meta_type_ref<void>)
        {
            return meta_string {"void"};
        }

        template <std::integral value_t>
        consteval any_meta_string auto to_string_impl(const value_t value)
        {
            return meta_string {"0"};
        }

        template <std::floating_point value_t>
        consteval any_meta_string auto to_string_impl(const value_t value)
        {
            return meta_string {"0.0"};
        }

        template <meta_enabled value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t>)
        {
            constexpr meta_adl<value_t> adl;
            constexpr auto func = []<meta_type type_v> { return type_v.name; };
            return with_meta_type(adl, func);
        }

        template <cv_qualified value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t>)
        {
            constexpr any_meta_string auto base_name = to_string_impl(meta_type_ref<std::remove_cv_t<value_t>> {});

            if constexpr (std::is_const_v<value_t> and std::is_volatile_v<value_t>)
            {
                return strings::concat(meta_string {"const volatile "}, base_name);
            }
            else if constexpr (std::is_const_v<value_t>)
            {
                return strings::concat(meta_string {"const "}, base_name);
            }
            else if constexpr (std::is_volatile_v<value_t>)
            {
                return strings::concat(meta_string {"volatile "}, base_name);
            }
            else
            {
                static_assert(false);
            }
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t&>)
        {
            return strings::concat(to_string_impl(meta_type_ref<value_t> {}), meta_string {"&"});
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t&&>)
        {
            return strings::concat(to_string_impl(meta_type_ref<value_t> {}), meta_string {"&&"});
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t*>)
        {
            return strings::concat(to_string_impl(meta_type_ref<value_t> {}), meta_string {"*"});
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t[]>)
        {
            return strings::concat(to_string_impl(meta_type_ref<value_t> {}), meta_string {"[]"});
        }

        template <typename value_t, std::size_t size_v>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t[size_v]>)
        {
            return strings::concat(
                to_string_impl(meta_type_ref<value_t> {}),
                meta_string {"["},
                to_string_impl(size_v),
                meta_string {"]"});
        }

        template <typename return_t, typename... args_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<return_t(args_t...)>)
        {
            // constexpr any_meta_string auto prefix = strings::concat(
            //     to_string_impl(meta_type_ref<return_t> {}), meta_string {"("});
            //
            // constexpr any_meta_string auto args = strings::concat(
            //     strings::concat(to_string_impl(meta_type_ref<args_t> {}), meta_string {", "})...);

            if constexpr (sizeof...(args_t) > 0)
            {
                constexpr any_meta_string auto args = strings::concat(
                    strings::concat(to_string_impl(meta_type_ref<args_t> {}), meta_string {", "})...);

                return strings::concat(
                    to_string_impl(meta_type_ref<return_t> {}),
                    meta_string {"("},
                    args.template resize<args.capacity() - 2>(),
                    meta_string {")"});
                // return strings::concat(
                // to_string_impl(meta_type_ref<return_t> {}), meta_string {"()"}
            }
            else
            {
                return strings::concat(to_string_impl(meta_type_ref<return_t> {}), meta_string {"()"});
            }
            // return strings::concat(
            //     to_string_impl(meta_type_ref<return_t> {}),
            //     meta_string {"("},
            //     (strings::concat(to_string_impl(meta_type_ref<args_t> {}), meta_string {", "}))...,
            //     meta_string {")"});
        }

        // template <typename value_t>
        // struct meta_type_name_impl;
        //
        // template <auto value_v>
        // struct meta_value_name_impl;
        //
        // template <auto value_v>
        // requires std::integral<decltype(value_v)> struct meta_value_name_impl
        // {
        //     static consteval any_meta_string auto get()
        //     {
        //         return "0";
        //     }
        // };
        //
        // template <auto value_v>
        // requires std::floating_point<decltype(value_v)> struct meta_value_name_impl
        // {
        //     static consteval any_meta_string auto get()
        //     {
        //         return "0.0";
        //     }
        // };
    }

    template <typename value_t>
    consteval any_meta_string auto to_string()
    {
        return detail::to_string_impl(meta_type_ref<value_t> {});
    }

    template <auto value_v>
    consteval any_meta_string auto to_string()
    {
        return detail::to_string_impl(value_v);
    }

    template <typename value_t>
    consteval any_meta_string auto to_string(value_t&& value)
    {
        return detail::to_string_impl(std::forward<value_t>(value));
    }

    // template <typename value_t>
    // consteval any_meta_string auto get_type_param_name()
    // {
    //     return detail::meta_type_name_impl<value_t>::get();
    // }
    //
    // template <auto value_v>
    // consteval any_meta_string auto get_value_param_name()
    // {
    //     if constexpr (any_meta_type_ref<decltype(value_v)>)
    //     {
    //         using value_t = typename decltype(value_v)::value_type;
    //         return detail::meta_type_name_impl<value_t>::get();
    //     }
    //     else
    //     {
    //         return detail::meta_value_name_impl<value_v>::get();
    //     }
    // }
}