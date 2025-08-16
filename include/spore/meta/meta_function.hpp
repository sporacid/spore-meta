#pragma once

#include "spore/meta/meta_argument.hpp"
#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_params.hpp"
#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_tuple.hpp"

#include <cstddef>
#include <type_traits>

namespace spore
{
    template <std::size_t name_v, typename function_t, typename return_t, any_meta_tuple arguments_t, any_meta_tuple params_t, any_meta_tuple attributes_t>
    struct meta_function;

    template <std::size_t name_v, typename function_t, typename return_t, any_meta_argument... arguments_t, any_meta_param... params_t, any_meta_attribute... attributes_t>
    struct meta_function<name_v, function_t, return_t, meta_tuple<arguments_t...>, meta_tuple<params_t...>, meta_tuple<attributes_t...>>
    {
        // static_assert(sizeof...(params_t) > 0 or std::is_invocable_v<function_t, typename arguments_t::value_type&&...>, "bad function argument types");
        // static_assert(sizeof...(params_t) > 0 or std::is_same_v<return_t, std::invoke_result_t<function_t, typename arguments_t::value_type&&...>>, "bad function return type");

        meta_string<name_v> name;
        function_t function;
        meta_type_ref<return_t> return_type;
        meta_tuple<arguments_t...> arguments;
        meta_tuple<params_t...> params;
        meta_tuple<attributes_t...> attributes;

        static constexpr bool is_template()
        {
            return sizeof...(params_t) > 0;
        }

        // template <typename... params_t, typename... args_t>
        // constexpr void invoke(auto& return_, args_t&&... args) const
        // {
        //     return_ = function.template operator()<params_t...>(std::forward<args_t>(args)...);
        // }

        template <typename... in_params_t, typename... args_t>
        constexpr auto invoke(args_t&&... args) const
        {
            // static_assert(sizeof...(params_t) == sizeof...(in_params_t));
            return function.template operator()<in_params_t...>(std::forward<args_t>(args)...);
        }

        //        template <typename... params_t>
        //        constexpr void invoke(return_t& return_, typename arguments_t::value_type&&... args) const
        //        {
        //            return_ = function.template operator()<params_t...>(std::forward<typename arguments_t::value_type>(args)...);
        //        }
        //
        //        template <typename... params_t>
        //        constexpr return_t invoke(typename arguments_t::value_type&&... args) const
        //        {
        //            return function.template operator()<params_t...>(std::forward<typename arguments_t::value_type>(args)...);
        //        }
      private:
        static consteval bool is_data_valid()
        {
            if constexpr (sizeof...(params_t) == 0)
            {
                constexpr bool is_args_valid = std::is_invocable_v<function_t, typename arguments_t::value_type&&...>;
                constexpr bool is_return_valid = std::is_same_v<return_t, std::invoke_result_t<function_t, typename arguments_t::value_type&&...>>;
                return is_args_valid and is_return_valid;
            }
            else
            {
                return true;
            }
        }

        template <typename... in_params_t, typename... args_t>
        static consteval bool is_template_data_valid(args_t&&...)
        {
            return true;
        }

        static_assert(is_data_valid(), "bad function data");
    };

    template <std::size_t name_v, typename function_t, typename return_t, any_meta_argument... arguments_t, any_meta_param... params_t, any_meta_attribute... attributes_t>
    meta_function(meta_string<name_v>, function_t, meta_type_ref<return_t>, meta_tuple<arguments_t...>, meta_tuple<params_t...>, meta_tuple<attributes_t...>)
        -> meta_function<name_v, function_t, return_t, meta_tuple<arguments_t...>, meta_tuple<params_t...>, meta_tuple<attributes_t...>>;

    template <typename>
    struct is_meta_function : std::false_type
    {
    };

    template <std::size_t name_v, typename function_t, typename return_t, any_meta_argument... arguments_t, any_meta_param... params_t, any_meta_attribute... attributes_t>
    struct is_meta_function<meta_function<name_v, function_t, return_t, meta_tuple<arguments_t...>, meta_tuple<params_t...>, meta_tuple<attributes_t...>>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_function_v = is_meta_function<value_t>::value;

    template <typename value_t>
    concept any_meta_function = is_meta_function_v<value_t>;
}