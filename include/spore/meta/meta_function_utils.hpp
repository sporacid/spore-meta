#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

#include "spore/meta/meta_placeholder.hpp"

namespace spore::meta::functions
{
    namespace detail
    {
        template <std::size_t index_v, typename input_tuple_t, typename output_tuple_t>
        consteval void check_assignable_impl()
        {
            if constexpr (index_v < std::tuple_size_v<output_tuple_t>)
            {
                static_assert(index_v < std::tuple_size_v<input_tuple_t>);

                using input_value_t = std::tuple_element_t<index_v, input_tuple_t>;
                using output_value_t = std::tuple_element_t<index_v, output_tuple_t>;
                using decayed_output_value_t = std::decay_t<output_value_t>;

                if constexpr (any_meta_placeholder<decayed_output_value_t>)
                {
                    if constexpr (decayed_output_value_t::variadic)
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

                check_assignable_impl<index_v + 1, input_tuple_t, output_tuple_t>();
            }
        }
    }

    template <typename input_tuple_t, typename output_tuple_t>
    consteval void check_assignable()
    {
        detail::check_assignable_impl<0, input_tuple_t, output_tuple_t>();
    }
}