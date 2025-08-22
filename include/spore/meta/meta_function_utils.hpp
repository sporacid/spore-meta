#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

#include "spore/meta/meta_parameter.hpp"

namespace spore::meta::functions
{
    namespace detail
    {
        template <typename input_t, typename output_t>
        struct is_assignable_impl : std::is_convertible<input_t, output_t>
        {
        };

        //        template <typename input_t, bool variadic_v>
        //        struct is_assignable_impl<input_t, meta_placeholder<variadic_v>&&> : std::true_type
        //        {
        //        };
        //
        //        template <typename input_t, bool variadic_v>
        //        struct is_assignable_impl<input_t, meta_placeholder<variadic_v>> : std::true_type
        //        {
        //        };
        //
        //        template <typename input_t, bool variadic_v>
        //        struct is_assignable_impl<input_t, const meta_placeholder<variadic_v>> : std::true_type
        //        {
        //        };
        //
        //        template <typename input_t, bool variadic_v>
        //        struct is_assignable_impl<input_t, const meta_placeholder<variadic_v>&> : std::true_type
        //        {
        //        };
        //
        //        template <typename output_t, bool variadic_v>
        //        struct is_assignable_impl<meta_placeholder<variadic_v>, output_t>
        //        {
        //        };

        template <std::size_t index_v, typename input_tuple_t, typename output_tuple_t, any_meta_parameter... parameters_t>
        constexpr void check_assignable_impl(const meta_tuple<parameters_t...>& parameters)
        {
            if constexpr (index_v < std::tuple_size_v<output_tuple_t>)
            {
                static_assert(index_v < std::tuple_size_v<input_tuple_t>);

                using input_value_t = std::tuple_element_t<index_v, input_tuple_t>;
                using output_value_t = std::tuple_element_t<index_v, output_tuple_t>;
                using decayed_output_value_t = std::decay_t<output_value_t>;

                if constexpr (any_meta_parameter_ref<decayed_output_value_t>)
                {
                    const any_meta_parameter auto& parameter = parameters.template at<decayed_output_value_t::index>();

                    if (parameter.variadic)
                    {
                        // If placeholder is variadic, then rest input of input is a parameter pack.
                        // TODO @sporacid Should probably test all inputs against template, e.g. value_t&... would require all l-value.
                        return;
                    }
                }
                else
                {
                    static_assert(std::is_convertible_v<input_value_t, output_value_t>);
                }

                check_assignable_impl<index_v + 1, input_tuple_t, output_tuple_t>(parameters);
            }
        }
    }

    template <typename input_tuple_t, typename output_tuple_t, any_meta_parameter... parameters_t>
    constexpr void check_assignable(const meta_tuple<parameters_t...>& parameters)
    {
        detail::check_assignable_impl<0, input_tuple_t, output_tuple_t>(parameters);
    }
}