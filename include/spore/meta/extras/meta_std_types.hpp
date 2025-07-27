#pragma once

#include "spore/meta/extras/meta_detail_macros.hpp"
#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_type.hpp"
#include "spore/meta/meta_type_utils.hpp"

#include <array>
#include <bitset>
#include <cstdint>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#define SPORE_META_TEMPLATE_PARAM(Index, Value) Value _t##Index,
#define SPORE_META_TEMPLATE_PARAM_NAME(Index, Value) _t##Index SPORE_META_IF(Index, SPORE_META_COMMA, SPORE_META_EMPTY)()
#define SPORE_META_TEMPLATE_PARAM_STRING(Index, Value) +spore::meta::types::to_string<_t##Index>() SPORE_META_IF(Index, + ", ", )

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

#define SPORE_META_DEFINE_STD_TEMPLATE(Name, ...)                                                                             \
    template <SPORE_META_FOR(SPORE_META_TEMPLATE_PARAM, __VA_ARGS__) typename func_t>                                         \
    constexpr auto with_meta_type(meta_adl<Name<SPORE_META_FOR(SPORE_META_TEMPLATE_PARAM_NAME, __VA_ARGS__)>>, func_t&& func) \
    {                                                                                                                         \
        constexpr meta_type type {                                                                                            \
            .name = meta_string {#Name} + "<" SPORE_META_FOR(SPORE_META_TEMPLATE_PARAM_STRING, __VA_ARGS__) + ">",            \
            .bases = meta_tuple {},                                                                                           \
            .fields = meta_tuple {},                                                                                          \
            .functions = meta_tuple {},                                                                                       \
            .constructors = meta_tuple {},                                                                                    \
            .attributes = meta_tuple {},                                                                                      \
        };                                                                                                                    \
                                                                                                                              \
        return func.template operator()<type>();                                                                              \
    }

namespace spore
{
    SPORE_META_DEFINE_STD_TYPE(char8_t)
    SPORE_META_DEFINE_STD_TYPE(char16_t)
    SPORE_META_DEFINE_STD_TYPE(char32_t)
    SPORE_META_DEFINE_STD_TYPE(wchar_t)

    SPORE_META_DEFINE_STD_TYPE(std::double_t)
    SPORE_META_DEFINE_STD_TYPE(std::float_t)
    SPORE_META_DEFINE_STD_TYPE(std::int8_t)
    SPORE_META_DEFINE_STD_TYPE(std::int16_t)
    SPORE_META_DEFINE_STD_TYPE(std::int32_t)
    SPORE_META_DEFINE_STD_TYPE(std::int64_t)
    SPORE_META_DEFINE_STD_TYPE(std::uint8_t)
    SPORE_META_DEFINE_STD_TYPE(std::uint16_t)
    SPORE_META_DEFINE_STD_TYPE(std::uint32_t)
    SPORE_META_DEFINE_STD_TYPE(std::uint64_t)
    SPORE_META_DEFINE_STD_TYPE(std::string)
    SPORE_META_DEFINE_STD_TYPE(std::string_view)
    SPORE_META_DEFINE_STD_TYPE(std::wstring)
    SPORE_META_DEFINE_STD_TYPE(std::wstring_view)
    SPORE_META_DEFINE_STD_TYPE(std::nullptr_t)
    SPORE_META_DEFINE_STD_TYPE(std::byte)

    SPORE_META_DEFINE_STD_TEMPLATE(std::allocator, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::array, typename, std::size_t)
    SPORE_META_DEFINE_STD_TEMPLATE(std::basic_string, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::basic_string_view, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::bitset, std::size_t)
    SPORE_META_DEFINE_STD_TEMPLATE(std::char_traits, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::deque, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::divides, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::equal_to, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::function, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::greater, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::greater_equal, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::hash, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::less, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::less_equal, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::list, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::map, typename, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::minus, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::multimap, typename, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::multiplies, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::multiset, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::not_equal_to, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::optional, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::pair, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::plus, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::priority_queue, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::queue, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::set, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::stack, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::unordered_map, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::unordered_set, typename, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::vector, typename, typename)

    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::tuple, typename, typename, typename, typename, typename, typename, typename, typename)

    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_STD_TEMPLATE(std::variant, typename, typename, typename, typename, typename, typename, typename, typename)
}