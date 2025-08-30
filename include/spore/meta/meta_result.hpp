#pragma once

#include <type_traits>
#include <variant>

namespace spore
{
    struct meta_continue
    {
    };

    struct meta_break
    {
    };

//    template <typename value_t>
//    struct meta_return
//    {
//        value_t value;
//
//        constexpr operator const value_t&() const&
//        {
//            return value;
//        }
//
//        constexpr operator value_t&() &
//        {
//            return value;
//        }
//
//        constexpr operator value_t&&() &&
//        {
//            return std::move(value);
//        }
//    };
//
//    template <typename value_t>
//    struct is_meta_return : std::false_type
//    {
//    };
//
//    template <typename value_t>
//    struct is_meta_return<meta_return<value_t>> : std::true_type
//    {
//    };
//
//    // clang-format off
//    template <typename value_t>
//    concept any_meta_result =
//        std::is_same_v<meta_continue, value_t> or
//        std::is_same_v<meta_break, value_t> or
//        is_meta_return<value_t>::value;
//    // clang-format on

    namespace meta
    {
        template <typename value_t>
        constexpr bool is_continue(value_t&&)
        {
            return std::is_same_v<meta_continue, std::decay_t<value_t>>;
        }

        template <typename... values_t>
        constexpr bool is_continue(const std::variant<values_t...>& variant)
        {
            return std::holds_alternative<meta_continue>(variant);
        }

        template <typename value_t>
        constexpr bool is_break(value_t&&)
        {
            return std::is_same_v<meta_break, std::decay_t<value_t>>;
        }

        template <typename... values_t>
        constexpr bool is_break(const std::variant<values_t...>& variant)
        {
            return std::holds_alternative<meta_break>(variant);
        }

//        template <typename value_t>
//        constexpr bool is_return(value_t&&)
//        {
//            return is_meta_return<std::decay_t<value_t>>::value;
//        }
//
//        template <typename... values_t>
//        constexpr bool is_return(const std::variant<values_t...>& variant)
//        {
//            constexpr auto visitor = []<typename value_t>(const value_t& value) {
//                return is_meta_return<std::decay_t<value_t>>::value;
//            };
//
//            return std::visit(visitor, variant);
//        }
    }

    //    namespace meta::detail
    //    {
    //        template <typename value_t, typename tuple_t>
    //        struct has_type;
    //
    //        template <typename value_t, typename... other_values_t>
    //        struct has_type<value_t, std::tuple<other_values_t...>>
    //            : std::disjunction<std::is_same<value_t, other_values_t>...>
    //        {
    //        };
    //    }

    //    template <typename... values_t>
    //    struct meta_result
    //    {
    //        std::variant<meta_continue, meta_break, values_t...> variant;
    //
    //        template <typename value_t>
    //        constexpr meta_result(std::in_place_t, value_t&& value)
    //            : variant(std::forward<value_t>(value))
    //        {
    //        }
    //
    //        constexpr bool is_continue() const
    //        {
    //            return std::holds_alternative<meta_continue>(variant);
    //        }
    //
    //        constexpr bool is_break() const
    //        {
    //            return std::holds_alternative<meta_break>(variant);
    //        }
    //
    //        template <typename value_t>
    //        constexpr bool has_value() const
    //        {
    //            return std::holds_alternative<value_t>();
    //        }
    //
    //        template <typename value_t>
    //        constexpr const value_t& get_value() const&
    //        {
    //            return std::get<value_t>(variant);
    //        }
    //
    //        template <typename value_t>
    //        constexpr value_t& get_value() &
    //        {
    //            return std::get<value_t>(variant);
    //        }
    //
    //        template <typename value_t>
    //        constexpr value_t&& get_value() &&
    //        {
    //            return std::move(std::get<value_t>(variant));
    //        }
    //
    //        template <typename value_t>
    //        constexpr operator const value_t&() const&
    //        {
    //            return get_value<value_t>();
    //        }
    //
    //        template <typename value_t>
    //        constexpr operator value_t&() &
    //        {
    //            return get_value<value_t>();
    //        }
    //
    //        template <typename value_t>
    //        constexpr operator value_t&&() &&
    //        {
    //            return std::move(*this).template get_value<value_t>();
    //        }
    //    };
    //
    //    template <typename value_t>
    //    struct is_meta_result : std::false_type
    //    {
    //    };
    //
    //    template <typename... values_t>
    //    struct is_meta_result<meta_result<values_t...>> : std::true_type
    //    {
    //    };
    //
    //    template <typename value_t>
    //    concept any_meta_result = is_meta_result<value_t>::value;
    //
    //    namespace meta
    //    {
    //        namespace detail
    //        {
    //            template <typename value_t, typename... values_t>
    //            constexpr bool contains = (std::is_same_v<value_t, values_t> or ...);
    //
    //            template <typename...>
    //            struct unique;
    //
    //            template <>
    //            struct unique<>
    //            {
    //                using type = meta_result<>;
    //            };
    //
    //            template <typename, typename>
    //            struct concat;
    //
    //            template <typename... values_t, typename... other_values_t>
    //            struct concat<meta_result<values_t...>, meta_result<other_values_t...>>
    //            {
    //                using type = meta_result<values_t..., other_values_t...>;
    //            };
    //
    //            template <typename value_t, typename... values_t>
    //            struct unique<value_t, values_t...>
    //            {
    //                using tail_unique = typename unique<values_t...>::type;
    //                using type = std::conditional_t<
    //                    contains<value_t, values_t...>,
    //                    tail_unique,
    //                    typename concat<meta_result<value_t>, tail_unique>::type>;
    //            };
    //
    //            template <typename, typename>
    //            struct compose_result_type;
    //
    //            template <typename... values_t, typename... other_values_t>
    //            struct compose_result_type<meta_result<values_t...>, meta_result<other_values_t...>>
    //            {
    //                using type = typename unique<values_t..., other_values_t...>::type;
    //            };
    //        }
    //
    //        template <typename... values_t>
    //        constexpr meta_result<values_t...> continue_()
    //        {
    //            return meta_result<values_t...> {std::in_place, meta_continue {}};
    //        }
    //
    //        template <typename... values_t>
    //        constexpr meta_result<values_t...> break_()
    //        {
    //            return meta_result<values_t...> {std::in_place, meta_break {}};
    //        }
    //
    //        template <typename value_t>
    //        constexpr meta_result<value_t> return_(value_t&& value)
    //        {
    //            return meta_result<value_t> {std::in_place, std::forward<value_t>(value)};
    //        }
    //
    //        template <any_meta_result other_result_t, typename... values_t>
    //        constexpr auto compose_result(meta_result<values_t...> result)
    //        {
    //            using composed_result_t = typename detail::compose_result_type<meta_result<values_t...>, other_result_t>::type;
    //
    //            constexpr auto visitor = []<typename other_value_t>(other_value_t&& value) {
    //                return composed_result_t {std::in_place, std::forward<other_value_t>(value)};
    //            };
    //
    //            return std::visit(visitor, std::move(result.variant));
    //        }
    //    }
}