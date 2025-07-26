#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_type.hpp"
#include "spore/meta/codegen/meta_codegen_detail.hpp"

#include <cstdint>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define SPORE_META_NUM_ARGS(...) SPORE_META_NUM_ARGS_(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define SPORE_META_NUM_ARGS_(_8, _7, _6, _5, _4, _3, _2, _1, N, ...) N

#define SPORE_META_CONCAT(A, B) SPORE_META_CONCAT_(A, B)
#define SPORE_META_CONCAT_(A, B) A##B

#define SPORE_META_FOR_N(Num, Macro, ...) SPORE_META_CONCAT(SPORE_META_FOR_, Num)(Macro, __VA_ARGS__)
#define SPORE_META_FOR(Macro, ...) SPORE_META_FOR_N(SPORE_META_NUM_ARGS(__VA_ARGS__), Macro, __VA_ARGS__)

#define SPORE_META_FOR_1(Macro, Value, ...) Macro(0, Value)
#define SPORE_META_FOR_2(Macro, Value, ...) Macro(1, Value) SPORE_META_FOR_1(Macro, __VA_ARGS__)
#define SPORE_META_FOR_3(Macro, Value, ...) Macro(2, Value) SPORE_META_FOR_2(Macro, __VA_ARGS__)
#define SPORE_META_FOR_4(Macro, Value, ...) Macro(3, Value) SPORE_META_FOR_3(Macro, __VA_ARGS__)
#define SPORE_META_FOR_5(Macro, Value, ...) Macro(4, Value) SPORE_META_FOR_4(Macro, __VA_ARGS__)
#define SPORE_META_FOR_6(Macro, Value, ...) Macro(5, Value) SPORE_META_FOR_5(Macro, __VA_ARGS__)
#define SPORE_META_FOR_7(Macro, Value, ...) Macro(6, Value) SPORE_META_FOR_6(Macro, __VA_ARGS__)
#define SPORE_META_FOR_8(Macro, Value, ...) Macro(7, Value) SPORE_META_FOR_7(Macro, __VA_ARGS__)

#define SPORE_META_BOOL(Value) SPORE_META_CONCAT(SPORE_META_BOOL_, Value)
#define SPORE_META_BOOL_0 0
#define SPORE_META_BOOL_1 1
#define SPORE_META_BOOL_2 1
#define SPORE_META_BOOL_3 1
#define SPORE_META_BOOL_4 1
#define SPORE_META_BOOL_5 1
#define SPORE_META_BOOL_6 1
#define SPORE_META_BOOL_7 1

#define SPORE_META_IF_0(True, False) False
#define SPORE_META_IF_1(True, False) True
#define SPORE_META_IF_BOOL(Bool, True, False) SPORE_META_CONCAT(SPORE_META_IF_, Bool)(True, False)
#define SPORE_META_IF(Condition, True, False) SPORE_META_IF_BOOL(SPORE_META_CONCAT(SPORE_META_BOOL_, Condition), True, False)

#define SPORE_META_DEFINE_STD_TYPE(Name)                                \
    template <typename func_t>                                          \
    constexpr auto with_meta_type(spore::meta_adl<Name>, func_t&& func) \
    {                                                                   \
        constexpr spore::meta_type type {                               \
            .name = meta_string {#Name},                                \
            .bases = meta_tuple {},                                     \
            .fields = meta_tuple {},                                    \
            .functions = meta_tuple {},                                 \
            .constructors = meta_tuple {},                              \
            .attributes = meta_tuple {},                                \
        };                                                              \
                                                                        \
        return func.template operator()<type>();                        \
    }

#define SPORE_META_EMPTY()
#define SPORE_META_COMMA() ,

#define SPORE_META_TEMPLATE_PARAM(Index, Value) Value _t##Index,
#define SPORE_META_TEMPLATE_PARAM_NAME(Index, Value) _t##Index SPORE_META_IF(Index, SPORE_META_COMMA, SPORE_META_EMPTY)()
#define SPORE_META_TEMPLATE_PARAM_STRING(Index, Value) , spore::meta::codegen::detail::meta_name_impl<_t##Index>::get(), meta_string{ SPORE_META_IF(Index, ", ", "")}

#define SPORE_META_DEFINE_STD_TEMPLATE(Name, ...)                                                                             \
    template <SPORE_META_FOR(SPORE_META_TEMPLATE_PARAM, __VA_ARGS__) typename func_t>                                         \
    constexpr auto with_meta_type(meta_adl<Name<SPORE_META_FOR(SPORE_META_TEMPLATE_PARAM_NAME, __VA_ARGS__)>>, func_t&& func) \
    {                                                                                                                         \
        constexpr meta_type type {                                                                                            \
            .name = meta::strings::concat(                                                                                    \
                meta_string {#Name},                                                                                          \
                meta_string {"<"} SPORE_META_FOR(SPORE_META_TEMPLATE_PARAM_STRING, __VA_ARGS__),                              \
                meta_string {">"}),                                                                                           \
            .bases = meta_tuple {},                                                                                           \
            .fields = meta_tuple {},                                                                                          \
            .functions = meta_tuple {},                                                                                       \
            .constructors = meta_tuple {},                                                                                    \
            .attributes = meta_tuple {},                                                                                      \
        };                                                                                                                    \
                                                                                                                              \
        return func.template operator()<type>();                                                                              \
    };

namespace spore
{
    SPORE_META_DEFINE_STD_TYPE(std::double_t);
    SPORE_META_DEFINE_STD_TYPE(std::float_t);
    SPORE_META_DEFINE_STD_TYPE(std::int8_t);
    SPORE_META_DEFINE_STD_TYPE(std::int16_t);
    SPORE_META_DEFINE_STD_TYPE(std::int32_t);
    SPORE_META_DEFINE_STD_TYPE(std::int64_t);
    SPORE_META_DEFINE_STD_TYPE(std::string);
    SPORE_META_DEFINE_STD_TYPE(std::string_view);
    SPORE_META_DEFINE_STD_TYPE(std::uint8_t);
    SPORE_META_DEFINE_STD_TYPE(std::uint16_t);
    SPORE_META_DEFINE_STD_TYPE(std::uint32_t);
    SPORE_META_DEFINE_STD_TYPE(std::uint64_t);

    SPORE_META_DEFINE_STD_TEMPLATE(std::allocator, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::basic_string, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::basic_string_view, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::char_traits, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::deque, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::equal_to, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::greater, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::hash, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::less, typename);
    // SPORE_META_DEFINE_STD_TEMPLATE(std::map, typename, typename, typename, typename);
    // SPORE_META_DEFINE_STD_TEMPLATE(std::multimap, typename, typename, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::multiset, typename, typename, typename);
    // SPORE_META_DEFINE_STD_TEMPLATE(std::pair, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::set, typename, typename, typename);
    // SPORE_META_DEFINE_STD_TEMPLATE(std::unordered_map, typename, typename, typename, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::unordered_set, typename, typename, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::vector, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::stack, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::list, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::function, typename);

    // SPORE_META_DEFINE_STD_TEMPLATE(std::array, typename, std::size_t);

    // TODO @sporacid Implement variadic parameters

    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename, typename, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename, typename, typename, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename, typename, typename, typename, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename, typename, typename, typename, typename, typename, typename);

    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename, typename, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename, typename, typename, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename, typename, typename, typename, typename, typename);
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename, typename, typename, typename, typename, typename, typename);
}