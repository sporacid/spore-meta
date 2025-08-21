#pragma once

#include "spore/meta.hpp"

#include <iostream>

#include SPORE_META_GENERATED("spore/examples/templates.meta.hpp")

namespace spore::examples::templates
{
    template <std::size_t size_v, typename value_t, typename... other_value_t>
    struct SPORE_META_TYPE() templated_type
    {
        value_t value;
        std::tuple<other_value_t...> other_values;

        template <typename arg_t, typename... other_arg_t>
        SPORE_META_CONSTRUCTOR()
        templated_type(arg_t&& arg, other_arg_t&&... other_args)
            : value {static_cast<value_t>(std::forward<arg_t>(arg))},
              other_values {static_cast<other_value_t>(std::forward<other_arg_t>(other_args))...}
        {
            std::cout << "Templated constructor invoked with " << std::to_string(arg);
            ((std::cout << " and " << std::to_string(other_args)), ...);
            std::cout << std::endl;
        }

        template <typename result_t, typename arg_t>
        SPORE_META_FUNCTION()
        result_t templated_function(arg_t&& arg) const
        {
            std::cout << "Templated function invoked with " << std::to_string(arg) << std::endl;
            return static_cast<result_t>(arg);
        }
    };
}

#include SPORE_META_GENERATED("spore/examples/templates.meta.inl")