#pragma once

#include "spore/meta/meta_argument.hpp"
#include "spore/meta/meta_attribute.hpp"
#include "spore/meta/meta_params.hpp"
#include "spore/meta/meta_string.hpp"
#include "spore/meta/meta_tuple.hpp"

#include <cstddef>
#include <type_traits>

#define SPORE_META_TEMPLATE_PARAM_TYPES (typename, auto)
#define SPORE_META_TEMPLATE_PARAM_TYPES_0 typename
#define SPORE_META_TEMPLATE_PARAM_TYPES_1 auto

#include "boost/preprocessor.hpp"

// Define the parameter kinds
// #define PARAM_KINDS (typename)(auto)
//
//// Tag each kind with its index
// #define TAG_INDEX(s, data, i, elem) (elem)(i)
//
//// Format a tagged parameter (kind, index) → typename T0, auto T1, etc.
//   #define FORMAT_PARAM(s, data, elem) \
//BOOST_PP_TUPLE_ELEM(2, 0, elem) BOOST_PP_CAT(T, BOOST_PP_TUPLE_ELEM(2, 1, elem))
//
//// Generate a single function template for a given parameter kind sequence
//   #def ine GENERATE_TEMPLATE(r, param_seq) \
//template<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(FORMAT_PARAM, _, \
//    BOOST_PP_SEQ_TRANSFORM(TAG_INDEX, _, param_seq)))> \
//void BOOST_PP_CAT(f_, BOOST_PP_SEQ_CAT(param_seq))();
//
//// Helper to repeat PARAM_KINDS N times as a sequence of sequences
// #define PARAM_KINDS_TUPLE(z, n, _) (PARAM_KINDS)
//
//// Generate all permutations for a given N
//   #define REPEAT_PRODUCT(z, n, _) \
//BOOST_PP_SEQ_FOR_EACH_PRODUCT(GENERATE_TEMPLATE, BOOST_PP_REPEAT(n, PARAM_KINDS_TUPLE, ~))

// Generate for N = 1 to 8
// BOOST_PP_REPEAT_FROM_TO(1, 9, REPEAT_PRODUCT, ~)

// #define PARAM_KINDS (typename)(auto)
// #define FORMAT_PARAM(s, data, i, kind) kind BOOST_PP_CAT(T, i)
// #define FORMAT_PARAM_NAME(s, data, i, kind) BOOST_PP_CAT(T, i)
//
// #define GENERATE_TEMPLATE(r, data) void f();
//     // template <BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(FORMAT_PARAM, _, param_seq)), typename... args_t> \
//    // constexpr auto invoke(args_t&&... args); /* { return func.template operator()<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(FORMAT_PARAM_NAME, _, param_seq))>(std::forward<args_t>(args)...); }*/
//
// #define PARAM_KINDS_TUPLE(z, n, _) (PARAM_KINDS)
// #def ine REPEAT_PRODUCT(z, n, _) \
//    BOOST_PP_SEQ_FOR_EACH_PRODUCT(GENERATE_TEMPLATE, BOOST_PP_REPEAT(n, PARAM_KINDS_TUPLE, ~))

//#define SPORE_META_TEMPLATE_INVOKE(R, Result) \
//    /* template <BOOST_PP_SEQ_ENUM(Result)> */
//
// #define SPORE_META_TEMPLATE_PARAM_KINDS (typename)(auto)
// #define SPORE_META_TEMPLATE_PARAM_KIND_PRODUCT(Z, N, _) \
//    BOOST_PP_SEQ_FOR_EACH_PRODUCT(SPORE_META_TEMPLATE_INVOKE, BOOST_PP_REPEAT(N, SPORE_META_TEMPLATE_PARAM_KINDS, ~))

// Define the parameter kinds
#define PARAM_KINDS (typename)(auto)

//# // Tag each kind with its index
//# #define TAG_INDEX(s, data, i, elem) (elem)(i)
//#
//# // Format a tagged parameter (kind, index) → typename T0, auto T1, etc.
//# #define FORMAT_PARAM(s, data, elem) \
//# BOOST_PP_TUPLE_ELEM(2, 0, elem) BOOST_PP_CAT(T, BOOST_PP_TUPLE_ELEM(2, 1, elem))

// BOOST_PP_CAT(BOOST_PP_IF_, i)() BOOST_PP_COMMA_IF(i)
#define FORMAT_PARAM(S, Data, Index, Element) BOOST_PP_COMMA_IF(Index)  Element BOOST_PP_CAT(_t, Index)
#define FORMAT_PARAM_NAME(S, Data, Index, Element) BOOST_PP_COMMA_IF(Index) BOOST_PP_CAT(_t, Index)

// Generate a single function template for a given parameter kind sequence
#define GENERATE_TEMPLATE(R, Seq)                                                                                          \
    template <BOOST_PP_SEQ_FOR_EACH_I(FORMAT_PARAM, _, Seq), typename... args_t>                                            \
    constexpr auto invoke(args_t&&... args) const                                                                          \
    {                                                                                                                      \
        return function.template operator()<BOOST_PP_SEQ_FOR_EACH_I(FORMAT_PARAM_NAME, _, Seq)>(std::forward<args_t>(args)...); \
    }

// Helper to repeat PARAM_KINDS N times as a sequence of sequences
#define PARAM_KINDS_TUPLE(z, n, _) (PARAM_KINDS)

// Generate all permutations for N = 1 to 8
#define REPEAT_PRODUCT(z, n, _) \
    BOOST_PP_SEQ_FOR_EACH_PRODUCT(GENERATE_TEMPLATE, BOOST_PP_REPEAT(n, PARAM_KINDS_TUPLE, ~))

// BOOST_PP_REPEAT_FROM_TO(1, 9, REPEAT_PRODUCT, ~)

namespace spore
{
    template <std::size_t name_v, typename function_t, typename return_t, any_meta_tuple arguments_t, any_meta_tuple params_t, any_meta_tuple attributes_t>
    struct meta_function;

    template <std::size_t name_v, typename function_t, typename return_t, any_meta_argument... arguments_t, any_meta_param... params_t, any_meta_attribute... attributes_t>
    struct meta_function<name_v, function_t, return_t, meta_tuple<arguments_t...>, meta_tuple<params_t...>, meta_tuple<attributes_t...>>
    {
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

        template <typename... args_t>
        constexpr auto invoke(args_t&&... args) const
        {
            return function.operator()(std::forward<args_t>(args)...);
        }

        BOOST_PP_REPEAT_FROM_TO(1, 4, REPEAT_PRODUCT, ~)

        // template <typename... in_params_t, typename... args_t>
        // constexpr return_t invoke(args_t&&... args) const
        // {
        //     if constexpr (is_template())
        //     {
        //         // static_assert(std::is_invocable_v<function_t::template operator()<in_params_t...>, args_t&&>);
        //         // static_assert(std::is_same_v<return_t, std::invoke_result_t<function_t::template operator()<in_params_t...>>>);
        //
        //     }
        //     else
        //     {
        //
        //     }
        //     return std::apply(function, std::tie(std::forward<args_t>(args)...));
        // }

        //        template <typename... in_params_t, typename... args_t>
        //        constexpr return_t invoke(args_t&&... args) const
        //        {
        //            return std::apply(function, std::tie(std::forward<args_t>(args)...));
        //        }
        //
        //        template <typename... args_t>
        //        constexpr return_type invoke(args_t&&... args) const
        //        {
        //            meta::functions::check_assignable<std::tuple<args_t&&...>, args_type>();
        //            return std::apply(function, std::tie(std::forward<args_t>(args)...));
        //        }
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