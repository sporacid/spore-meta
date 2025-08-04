#pragma once

#include "spore/meta.hpp"

#include <iostream>

#include SPORE_META_GENERATED("spore/examples/templates.meta.hpp")

namespace spore::examples::templates
{
    template <std::size_t size_v, typename value_t, typename... other_value_t>
    struct SPORE_META_TYPE() templated_type
    {
        template <typename in_value_t, typename... in_other_value_t>
        SPORE_META_CONSTRUCTOR()
        templated_type(in_value_t&&, in_other_value_t&&...)
        {
            std::cout << "Templated constructor invoked" << std::endl;
        }

        template <typename result_t>
        SPORE_META_FUNCTION()
        result_t templated_function()
        {
            std::cout << "Templated function invoked" << std::endl;
            return result_t();
        }
    };
}

#include SPORE_META_GENERATED("spore/examples/templates.meta.inl")