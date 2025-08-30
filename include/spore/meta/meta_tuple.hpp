#pragma once

#include "spore/meta/meta_invalid.hpp"
#include "spore/meta/meta_result.hpp"

#include <cstddef>
#include <type_traits>
#include <variant>

namespace spore
{
    template <typename...>
    struct meta_tuple;

    template <>
    struct meta_tuple<>
    {
        constexpr std::size_t size() const
        {
            return 0ULL;
        }

        template <std::size_t>
        constexpr auto at() const
        {
            return meta_invalid();
        }
    };

    template <typename value_t, typename... values_t>
    struct meta_tuple<value_t, values_t...>
    {
        value_t value;
        meta_tuple<values_t...> rest;

        constexpr meta_tuple() = default;

        explicit constexpr meta_tuple(value_t&& value, values_t&&... rest)
            : value(std::forward<value_t>(value)),
              rest(std::forward<values_t>(rest)...) {}

        constexpr std::size_t size() const
        {
            return 1ULL + sizeof...(values_t);
        }

        template <std::size_t index_v>
        constexpr auto at() const
        {
            if constexpr (index_v == 0)
            {
                return value;
            }
            else
            {
                return rest.template at<index_v - 1>();
            }
        }
    };

    template <typename... values_t>
    meta_tuple(values_t&&...)
        -> meta_tuple<values_t...>;

    namespace meta::tuples
    {
        namespace detail
        {
            template <typename>
            struct deduce_find_variant_type;

            template <typename... args_t>
            struct deduce_find_variant_type<meta_tuple<args_t...>>
            {
                using type = std::variant<meta_invalid, args_t...>;
            };

            template <typename, typename>
            struct deduce_for_each_variant_type;

            template <typename value_t, typename... values_t>
            struct deduce_for_each_variant_type<value_t, std::variant<values_t...>>
            {
                using type = std::variant<meta_continue, meta_break, value_t, values_t...>;
            };

            template <typename value_t, typename... values_t>
            struct deduce_for_each_variant_type<std::variant<values_t...>, value_t>
            {
                using type = std::variant<meta_continue, meta_break, values_t..., value_t>;
            };

            template <typename... values_t, typename... other_values_t>
            struct deduce_for_each_variant_type<std::variant<values_t...>, std::variant<other_values_t...>>
            {
                using type = std::variant<meta_continue, meta_break, values_t..., other_values_t...>;
            };

            template <auto arg_v, typename func_t>
            constexpr auto is_constexpr_invocable_impl(func_t func)
                -> std::integral_constant<bool, (func.template operator()<arg_v>(), true)>
            {
                return {};
            }

            template <auto arg_v>
            constexpr std::false_type is_constexpr_invocable_impl(...)
            {
                return {};
            }

            template <typename func_t, auto arg_v>
            consteval bool is_constexpr_invocable()
            {
                if constexpr (std::is_default_constructible_v<func_t>)
                {
                    constexpr auto value = is_constexpr_invocable_impl<arg_v>(func_t {});
                    return decltype(value)::value;
                }
                else
                {
                    return false;
                }
            }

            template <std::size_t index_v, meta_tuple tuple_v, typename func_t>
            constexpr auto for_each_impl(func_t&& func)
            {
                if constexpr (index_v < tuple_v.size())
                {
                    constexpr auto value = tuple_v.template at<index_v>();

                    if constexpr (is_constexpr_invocable<std::decay_t<func_t>, value>())
                    {
                        constexpr auto result = std::decay_t<func_t> {}.template operator()<value>();

                        if constexpr (meta::is_continue(result))
                        {
                            return for_each_impl<index_v + 1, tuple_v>(func);
                        }
                        else
                        {
                            return result;
                        }
                    }
                    else
                    {
                        auto result = func.template operator()<value>();

                        using variant_t = typename deduce_for_each_variant_type<decltype(result), decltype(for_each_impl<index_v + 1, tuple_v>(func))>::type;

                        if (meta::is_continue(result))
                        {
                            return variant_t {for_each_impl<index_v + 1, tuple_v>(func)};
                        }
                        else
                        {
                            return variant_t {std::move(result)};
                        }
                    }
                    //                    else
                    //                    {
                    //                        (void) func.template operator()<value>();
                    //
                    //                        return for_each_impl<index_v + 1, tuple_v>(func);
                    //                    }
                }
                else
                {
                    return meta_continue {};
                }
            }

            template <std::size_t index_v, meta_tuple tuple_v, typename predicate_t>
            constexpr auto find_impl(predicate_t&& predicate)
            {
                if constexpr (index_v < tuple_v.size())
                {
                    constexpr auto value = tuple_v.template at<index_v>();

                    if constexpr (is_constexpr_invocable<std::decay_t<predicate_t>, value>())
                    {
                        if constexpr (std::decay_t<predicate_t> {}.template operator()<value>())
                        {
                            return value;
                        }
                        else
                        {
                            return find_impl<index_v + 1, tuple_v>(predicate);
                        }
                    }
                    else
                    {
                        using variant_t = typename deduce_find_variant_type<decltype(tuple_v)>::type;

                        if (predicate.template operator()<value>())
                        {
                            return variant_t {value};
                        }

                        return variant_t {find_impl<index_v + 1, tuple_v>(predicate)};
                    }
                }
                else
                {
                    return meta_invalid {};
                }
            }

            template <std::size_t index_v, meta_tuple tuple_v, typename predicate_t>
            constexpr bool all_of_impl(predicate_t&& predicate)
            {
                if constexpr (index_v < tuple_v.size())
                {
                    constexpr auto value = tuple_v.template at<index_v>();

                    if constexpr (is_constexpr_invocable<std::decay_t<predicate_t>, value>())
                    {
                        if constexpr (std::decay_t<predicate_t> {}.template operator()<value>())
                        {
                            return all_of_impl<index_v + 1, tuple_v>(predicate);
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return predicate.template operator()<value>() and all_of_impl<index_v + 1, tuple_v>(predicate);
                    }
                }
                else
                {
                    return true;
                }
            }
        }

        template <meta_tuple tuple_v, typename func_t>
        constexpr auto for_each(func_t&& func)
        {
            return detail::for_each_impl<0, tuple_v>(func);
        }

        template <meta_tuple tuple_v, typename predicate_t>
        constexpr auto find(predicate_t&& predicate)
        {
            return detail::find_impl<0, tuple_v>(predicate);
        }

        template <meta_tuple tuple_v, typename predicate_t>
        constexpr bool all_of(predicate_t&& predicate)
        {
            return detail::all_of_impl<0, tuple_v>(predicate);
        }
    }

    template <typename>
    struct is_meta_tuple : std::false_type
    {
    };

    template <typename... values_t>
    struct is_meta_tuple<meta_tuple<values_t...>> : std::true_type
    {
    };

    template <typename value_t>
    concept any_meta_tuple = is_meta_tuple<value_t>::value;

    // clang-format off
    template <typename value_t, template <typename> typename concept_t>
    concept any_meta_tuple_of = any_meta_tuple<value_t>
        and meta::tuples::all_of<value_t{}>(
            []<auto value_v> { return concept_t<decltype(value_v)>::value; }
        );
    // clang-format on

    template <typename value_t>
    concept meta_tuple_convertible = any_meta_tuple<std::decay_t<value_t>>;
}