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

    template <typename value_t>
    struct meta_result;

    template <typename value_t>
    struct is_meta_result : std::false_type
    {
    };

    template <typename value_t>
    struct is_meta_result<meta_result<value_t>> : std::true_type
    {
    };

    template <typename value_t>
    concept any_meta_result = is_meta_result<value_t>::value;

    template <>
    struct meta_result<void>
    {
        std::variant<meta_continue, meta_break> variant;

        template <typename other_value_t>
        constexpr meta_result(other_value_t&& other_value) requires(not any_meta_result<std::decay_t<other_value_t>>)
            : variant(std::forward<other_value_t>(other_value))
        {
        }

        template <typename other_value_t>
        constexpr meta_result(meta_result<other_value_t> other)
        {
            const auto visitor = [&](auto&& other_value) {
                variant = std::forward<decltype(other_value)>(other_value);
            };

            std::visit(visitor, std::move(other.variant));
        }

        constexpr bool is_continue() const
        {
            return std::holds_alternative<meta_continue>(variant);
        }

        constexpr bool is_break() const
        {
            return std::holds_alternative<meta_break>(variant);
        }

        constexpr bool has_value() const
        {
            return false;
        }
    };

    template <typename value_t>
    struct meta_result
    {
        std::variant<meta_continue, meta_break, value_t> variant;

        template <typename other_value_t>
        constexpr meta_result(other_value_t&& other_value) requires(not any_meta_result<std::decay_t<other_value_t>>)
            : variant(std::forward<other_value_t>(other_value))
        {
        }

        template <typename other_value_t>
        constexpr meta_result(meta_result<other_value_t> other)
        {
            const auto visitor = [&](auto&& other_value) {
                variant = std::forward<decltype(other_value)>(other_value);
            };

            std::visit(visitor, std::move(other.variant));
        }

        constexpr bool is_continue() const
        {
            return std::holds_alternative<meta_continue>(variant);
        }

        constexpr bool is_break() const
        {
            return std::holds_alternative<meta_break>(variant);
        }

        constexpr bool has_value() const
        {
            return std::holds_alternative<value_t>(variant);
        }

        constexpr const value_t& get_value() const&
        {
            return std::get<value_t>(variant);
        }

        constexpr value_t& get_value() &
        {
            return std::get<value_t>(variant);
        }

        constexpr value_t&& get_value() &&
        {
            return std::get<value_t>(variant);
        }

        constexpr operator const value_t&() const&
        {
            return get_value();
        }

        constexpr operator value_t&() &
        {
            return get_value();
        }

        constexpr operator value_t&&() &&
        {
            return std::move(*this).get_value();
        }
    };

    template <typename value_t>
    meta_result(value_t&&)
        -> meta_result<std::decay_t<value_t>>;

#if 0
    template <typename... values_t>
    struct meta_result;

    template <typename value_t>
    struct is_meta_result : std::false_type
    {
    };

    template <typename... values_t>
    struct is_meta_result<meta_result<values_t...>> : std::true_type
    {
    };

    template <typename value_t>
    concept any_meta_result = is_meta_result<value_t>::value;

    template <typename... values_t>
    struct meta_result
    {
        std::variant<meta_continue, meta_break, values_t...> variant;

        template <typename value_t>
        constexpr meta_result(value_t&& value) requires(not any_meta_result<std::decay<value_t>>)
            : variant(std::forward<value_t>(value))
        {
        }

        template <typename... other_values_t>
        constexpr meta_result(meta_result<other_values_t...> other) requires(not std::is_same_v<meta_result, meta_result<other_values_t...>>)
        {
            const auto visitor = [&]<typename other_value_t>(other_value_t&& other_value) {
                variant = std::forward<other_value_t>(other_value);
            };

            std::visit(visitor, std::move(other));
        }

        template <typename value_t>
        constexpr meta_result& operator=(value_t&& value) requires(not any_meta_result<std::decay<value_t>>)
        {
            variant = std::forward<value_t>(value);
            return *this;
        }

        template <typename... other_values_t>
        constexpr meta_result& operator=(meta_result<other_values_t...> other) requires(not std::is_same_v<meta_result, meta_result<other_values_t...>>)
        {
            const auto visitor = [&]<typename other_value_t>(other_value_t&& other_value) {
                variant = std::forward<other_value_t>(other_value);
            };

            std::visit(visitor, std::move(other));
            return *this;
        }

        constexpr bool is_continue() const
        {
            return std::holds_alternative<meta_continue>(variant);
        }

        constexpr bool is_break() const
        {
            return std::holds_alternative<meta_break>(variant);
        }

        template <typename value_t>
        constexpr bool has_value() const
        {
            return std::holds_alternative<value_t>();
        }

        template <typename value_t>
        constexpr const value_t& get_value() const&
        {
            return std::get<value_t>(variant);
        }

        template <typename value_t>
        constexpr value_t& get_value() &
        {
            return std::get<value_t>(variant);
        }

        template <typename value_t>
        constexpr value_t&& get_value() &&
        {
            return std::move(std::get<value_t>(variant));
        }

        template <typename value_t>
        constexpr operator const value_t&() const&
        {
            return get_value<value_t>();
        }

        template <typename value_t>
        constexpr operator value_t&() &
        {
            return get_value<value_t>();
        }

        template <typename value_t>
        constexpr operator value_t&&() &&
        {
            return std::move(*this).template get_value<value_t>();
        }
    };

    namespace meta
    {
        namespace detail
        {
            template <typename value_t, typename... values_t>
            constexpr bool contains = (std::is_same_v<value_t, values_t> or ...);

            template <typename...>
            struct unique;

            template <>
            struct unique<>
            {
                using type = meta_result<>;
            };

            template <typename, typename>
            struct concat;

            template <typename... values_t, typename... other_values_t>
            struct concat<meta_result<values_t...>, meta_result<other_values_t...>>
            {
                using type = meta_result<values_t..., other_values_t...>;
            };

            template <typename value_t, typename... values_t>
            struct unique<value_t, values_t...>
            {
                using tail_unique = typename unique<values_t...>::type;
                using type = std::conditional_t<
                    contains<value_t, values_t...>,
                    tail_unique,
                    typename concat<meta_result<value_t>, tail_unique>::type>;
            };

            template <typename, typename>
            struct compose_result_type;

            template <typename... values_t, typename... other_values_t>
            struct compose_result_type<meta_result<values_t...>, meta_result<other_values_t...>>
            {
                using type = typename unique<values_t..., other_values_t...>::type;
            };
        }

        template <typename... values_t>
        constexpr meta_result<values_t...> continue_()
        {
            return meta_result<values_t...> {meta_continue {}};
        }

        template <typename... values_t>
        constexpr meta_result<values_t...> break_()
        {
            return meta_result<values_t...> {meta_break {}};
        }

        template <typename value_t>
        constexpr meta_result<value_t> return_(value_t&& value)
        {
            return meta_result<value_t> {std::forward<value_t>(value)};
        }

        template <any_meta_result other_result_t, typename... values_t>
        constexpr auto compose_result(meta_result<values_t...> result)
        {
            using composed_result_t = typename detail::compose_result_type<meta_result<values_t...>, other_result_t>::type;

            constexpr auto visitor = []<typename other_value_t>(other_value_t&& value) {
                return composed_result_t {std::in_place, std::forward<other_value_t>(value)};
            };

            return std::visit(visitor, std::move(result.variant));
        }
    }
#endif
}