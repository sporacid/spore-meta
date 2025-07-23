#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace spore::meta::utils
{
    namespace detail
    {
        template <std::size_t index_v, typename from_tuple_t, typename to_tuple_t>
        consteval void check_is_assignable_impl()
        {
            static_assert(std::tuple_size_v<from_tuple_t> == std::tuple_size_v<to_tuple_t>);

            if constexpr (index_v < std::tuple_size_v<from_tuple_t>)
            {
                static_assert(std::is_assignable_v<
                    std::tuple_element_t<index_v, to_tuple_t>,
                    std::tuple_element_t<index_v, from_tuple_t>>);

                check_is_assignable_impl<index_v + 1, from_tuple_t, to_tuple_t>();
            }
        }
    }

    template <typename from_tuple_t, typename to_tuple_t>
    consteval void check_is_assignable()
    {
        detail::check_is_assignable_impl<0, from_tuple_t, to_tuple_t>();
    }
}