#pragma once

#include "spore/meta/meta_enabled.hpp"
#include "spore/meta/meta_string.hpp"

namespace spore::meta::types
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

        template <std::integral auto value_v>
        consteval any_meta_string auto to_string_impl()
        {
            using value_t = decltype(value_v);

            if constexpr (std::is_signed_v<value_t> and value_v < 0)
            {
                return meta_string {"-"} + to_string_impl<-value_v>();
            }
            else if constexpr (value_v == 0)
            {
                return meta_string {"0"};
            }
            else
            {
                constexpr value_t digit = value_v % 10;
                constexpr value_t remainder = value_v / 10;

                constexpr char chars[] = {static_cast<char>('0' + digit), '\0'};

                if constexpr (remainder > 0)
                {
                    return to_string_impl<remainder>() + meta_string {chars};
                }
                else
                {
                    return meta_string {chars};
                }
            }
        }

        template <std::floating_point auto value_v>
        consteval any_meta_string auto to_string_impl()
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
                return meta_string {"const volatile "} + base_name;
            }
            else if constexpr (std::is_const_v<value_t>)
            {
                return meta_string {"const "} + base_name;
            }
            else if constexpr (std::is_volatile_v<value_t>)
            {
                return meta_string {"volatile "} + base_name;
            }
            else
            {
                static_assert(false);
            }
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t&>)
        {
            return to_string_impl(meta_type_ref<value_t> {}) + meta_string {"&"};
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t&&>)
        {
            return to_string_impl(meta_type_ref<value_t> {}) + meta_string {"&&"};
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t*>)
        {
            return to_string_impl(meta_type_ref<value_t> {}) + meta_string {"*"};
        }

        template <typename value_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t[]>)
        {
            return to_string_impl(meta_type_ref<value_t> {}) + meta_string {"[]"};
        }

        template <typename value_t, std::size_t size_v>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<value_t[size_v]>)
        {
            return to_string_impl(meta_type_ref<value_t> {}) + meta_string {"["} + to_string_impl<size_v>() + meta_string {"]"};
        }

        template <typename return_t, typename... args_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<return_t(args_t...)>)
        {
            constexpr any_meta_string auto return_ = to_string_impl(meta_type_ref<return_t> {});

            if constexpr (sizeof...(args_t) > 0)
            {
                constexpr any_meta_string auto args = (to_string_impl(meta_type_ref<args_t> {}) + ... + meta_string {", "});

                return return_ + meta_string {"("} + args.template resize<args.capacity() - 2>() + meta_string {")"};
            }
            else
            {
                return return_ + meta_string {"()"};
            }
        }

        template <typename return_t, typename... args_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<return_t (*)(args_t...)>)
        {
            constexpr any_meta_string auto return_ = to_string_impl(meta_type_ref<return_t> {});

            if constexpr (sizeof...(args_t) > 0)
            {
                constexpr any_meta_string auto args = (to_string_impl(meta_type_ref<args_t> {}) + ... + meta_string {", "});

                return return_ + meta_string {"(*)("} + args.template resize<args.capacity() - 2>() + meta_string {")"};
            }
            else
            {
                return return_ + meta_string {"(*)()"};
            }
        }

        template <typename return_t, typename... args_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<return_t (&)(args_t...)>)
        {
            constexpr any_meta_string auto return_ = to_string_impl(meta_type_ref<return_t> {});

            if constexpr (sizeof...(args_t) > 0)
            {
                constexpr any_meta_string auto args = (to_string_impl(meta_type_ref<args_t> {}) + ... + meta_string {", "});

                return return_ + meta_string {"(&)("} + args.template resize<args.capacity() - 2>() + meta_string {")"};
            }
            else
            {
                return return_ + meta_string {"(&)()"};
            }
        }

        template <typename this_t, typename return_t, typename... args_t>
        consteval any_meta_string auto to_string_impl(const meta_type_ref<return_t (this_t::*)(args_t...)>)
        {
            constexpr any_meta_string auto return_ = to_string_impl(meta_type_ref<return_t> {});
            constexpr any_meta_string auto this_ = to_string_impl(meta_type_ref<this_t> {});

            if constexpr (sizeof...(args_t) > 0)
            {
                constexpr any_meta_string auto args = (to_string_impl(meta_type_ref<args_t> {}) + ... + meta_string {", "});

                return return_ + meta_string {"("} + this_ + meta_string {"::*)("} + args.template resize<args.capacity() - 2>() + meta_string {")"};
            }
            else
            {
                return return_ + meta_string {"("} + this_ + meta_string {"::*)()"};
            }
        }
    }

    template <typename value_t>
    consteval any_meta_string auto to_string()
    {
        return detail::to_string_impl(meta_type_ref<value_t> {});
    }

    template <auto value_v>
    consteval any_meta_string auto to_string()
    {
        if constexpr (any_meta_type_ref<decltype(value_v)>)
        {
            return detail::to_string_impl(value_v);
        }
        else
        {
            return detail::to_string_impl<value_v>();
        }
    }

    template <meta_string separator, typename... values_t>
    consteval any_meta_string auto to_string()
    {
        constexpr any_meta_string auto joined = ((meta::types::to_string<values_t>() + separator) + ...);
        constexpr std::size_t new_size = joined.capacity() - separator.size();

        return joined.template resize<new_size>();
    }

    template <meta_string separator, auto... values_v>
    consteval any_meta_string auto to_string()
    {
        constexpr any_meta_string auto joined = ((meta::types::to_string<values_v>() + separator) + ...);
        constexpr std::size_t new_size = joined.capacity() - separator.size();

        return joined.template resize<new_size>();
    }
}