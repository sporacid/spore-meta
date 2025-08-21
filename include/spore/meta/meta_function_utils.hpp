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
            static_assert(std::tuple_size_v<input_tuple_t> == std::tuple_size_v<output_tuple_t>);

            if constexpr (index_v < std::tuple_size_v<input_tuple_t>)
            {
                using input_value_t = std::tuple_element_t<index_v, input_tuple_t>;
                using output_value_t = std::tuple_element_t<index_v, output_tuple_t>;

                if constexpr (not is_meta_placeholder_v<std::decay_t<output_value_t>>)
                {
                    static_assert(std::is_assignable_v<input_value_t, output_value_t>);
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