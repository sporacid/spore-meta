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

        explicit constexpr meta_tuple(value_t value, values_t... rest)
            : value(std::move(value)),
              rest(std::move(rest)...) {}

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
            struct deduce_variant_type;

            template <typename... args_t>
            struct deduce_variant_type<meta_tuple<args_t...>>
            {
                using type = std::variant<meta_invalid, args_t...>;
            };

            template <typename func_t, auto arg_v>
            constexpr bool is_constexpr_invocable()
            {
                using void_t = std::void_t<decltype(std::declval<func_t>().template operator()<arg_v>())>;

                if constexpr (std::is_default_constructible_v<func_t> and std::is_same_v<void, void_t>)
                {
                    return [=]() constexpr {
                        func_t func {};
                        func.template operator()<arg_v>();
                        return true;
                    }();
                }
                else
                {
                    return false;
                }
            }

            template <std::size_t index_v, meta_tuple tuple_v, typename func_t>
            constexpr meta_result auto for_each_impl(func_t&& func)
            {
                if constexpr (index_v < tuple_v.size())
                {
                    constexpr auto value = tuple_v.template at<index_v>();

                    using result_t = decltype(func.template operator()<value>());

                    if constexpr (meta_result<result_t>)
                    {
                        if constexpr (is_constexpr_invocable<std::decay_t<func_t>, value>())
                        {
                            constexpr meta_result auto result = std::decay_t<func_t> {}.template operator()<value>();

                            if constexpr (is_meta_continue_v<result_t>)
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
                            meta_result auto result = func.template operator()<value>();

                            if constexpr (is_meta_continue_v<result_t>)
                            {
                                return for_each_impl<index_v + 1, tuple_v>(func);
                            }
                            else
                            {
                                return result;
                            }
                        }
                    }
                    else
                    {
                        (void) func.template operator()<value>();

                        return for_each_impl<index_v + 1, tuple_v>(func);
                    }
                }
                else
                {
                    return meta_continue();
                }
            }

            template <std::size_t index_v, meta_tuple tuple_v, typename predicate_t>
            constexpr auto find_impl(predicate_t&& predicate)
            {
                if constexpr (index_v < tuple_v.size())
                {
                    constexpr auto value = tuple_v.template at<index_v>();

                    if constexpr (detail::is_constexpr_invocable<std::decay_t<predicate_t>, value>())
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
                        using variant_t = typename deduce_variant_type<decltype(tuple_v)>::type;

                        if (predicate.template operator()<value>())
                        {
                            return variant_t {value};
                        }

                        return variant_t {find_impl<index_v + 1, tuple_v>(predicate)};
                    }
                }
                else
                {
                    return meta_invalid();
                }
            }

            template <std::size_t index_v, meta_tuple tuple_v, typename predicate_t>
            constexpr bool all_of_impl(predicate_t&& predicate)
            {
                if constexpr (index_v < tuple_v.size())
                {
                    constexpr auto value = tuple_v.template at<index_v>();

                    if constexpr (detail::is_constexpr_invocable<std::decay_t<predicate_t>, value>())
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
        constexpr meta_result auto for_each(func_t&& func)
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
    constexpr bool is_meta_tuple_v = is_meta_tuple<value_t>::value;

    template <typename value_t>
    concept any_meta_tuple = is_meta_tuple_v<value_t>;

    // clang-format off
    template <typename value_t, template <typename> typename concept_t>
    concept any_meta_tuple_of = any_meta_tuple<value_t>
        and meta::tuples::all_of<value_t{}>(
            []<typename tuple_value_t> { return concept_t<tuple_value_t>::value; }
        );
    // clang-format on

    template <typename value_t>
    concept meta_tuple_convertible = any_meta_tuple<std::decay_t<value_t>>;
}