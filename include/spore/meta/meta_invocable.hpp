#pragma once

#include <functional>
#include <type_traits>

namespace spore
{
    // template <typename func_t, typename return_t, typename... args_t>
    // struct is_meta_invocable
    // {
    // };

    // template <auto value_v>
    // struct meta_constant
    // {
    //     static constexpr auto value = value_v;
    // };

    // template <typename value_t>
    // struct meta_value_param
    // {
    // };
//
    // struct meta_type_param
    // {
    // };

//    template <typename value_t>
//    struct meta_invocable_param;

//    template <auto value_v>
//    struct meta_invocable_param<meta_constant<value_v>>
//    {
//        static constexpr auto value = value_v;
//    };
//
//    template <typename value_t>
//    struct meta_invocable_param<meta_type_ref<value_t>>
//    {
//        using value = value_t;
//    };

//    template <typename func_t, typename... params_t>
//    struct meta_invocable
//    {
//        func_t func;
//
//        template <typename... args_t>
//        constexpr std::invoke_result_t<func_t, args_t&&...> operator()(args_t&&... args) const
//        {
//            return func.template operator()<meta_invocable_param<params_t>::value...>(std::forward<args_t>(args)...);
//        }
//    };
//
//    template <typename... params_t, typename func_t>
//    meta_invocable(func_t&&)
//        -> meta_invocable<std::decay_t<func_t>, params_t...>;
//
//    template <typename func_t>
//    struct meta_invocable2
//    {
//        func_t func;
//
//        template <typename... params_t, typename... args_t>
//        constexpr std::invoke_result_t<func_t, args_t&&...> operator()(args_t&&... args) const
//        {
//            return func.template operator()<params_t...>(std::forward<args_t>(args)...);
//        }
//    };

//    template <typename func_t, any_meta_tuple params_t>
//    struct meta_invocable3;
//
//    template <typename func_t, typename... params_t>
//    struct meta_invocable3<func_t, meta_tuple<params_t...>>
//    {
//        func_t func;
//        meta_tuple<params_t...> params;
//
//        template <typename... in_params_t, typename... args_t>
//        constexpr auto invoke(args_t&&... args) const
//        {
//            return func.template operator()<in_params_t...>(std::forward<args_t>(args)...);
//        }
//    };
//
//    template <typename func_t, typename... params_t>
//    meta_invocable3(func_t, meta_tuple<params_t...>)
//        -> meta_invocable3<func_t, meta_tuple<params_t...>>;

    //    template <typename func_t, typename... params_t>
    //    struct meta_invocable
    //    {
    //        std::function<signature_t> func;
    //
    //        constexpr auto operator()()
    //        {
    //        }
    //    };
    //
    //    template <typename signature_t>
    //    struct meta_invocable;
}