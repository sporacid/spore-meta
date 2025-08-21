#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_string_convert.hpp"
#include "spore/meta/meta_type.hpp"

#include "spore/meta/extras/meta_extras_macros.hpp"

// TODO @sporacid Is there a better way than including all of std?

#include <array>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <filesystem>
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

namespace spore
{
    SPORE_META_DEFINE_TYPE(char)
    SPORE_META_DEFINE_TYPE(char8_t)
    SPORE_META_DEFINE_TYPE(char16_t)
    SPORE_META_DEFINE_TYPE(char32_t)
    SPORE_META_DEFINE_TYPE(wchar_t)

    SPORE_META_DEFINE_TYPE(std::double_t)
    SPORE_META_DEFINE_TYPE(std::float_t)
    SPORE_META_DEFINE_TYPE(std::int8_t)
    SPORE_META_DEFINE_TYPE(std::int16_t)
    SPORE_META_DEFINE_TYPE(std::int32_t)
    SPORE_META_DEFINE_TYPE(std::int64_t)
    SPORE_META_DEFINE_TYPE(std::uint8_t)
    SPORE_META_DEFINE_TYPE(std::uint16_t)
    SPORE_META_DEFINE_TYPE(std::uint32_t)
    SPORE_META_DEFINE_TYPE(std::uint64_t)
    SPORE_META_DEFINE_TYPE(std::string)
    SPORE_META_DEFINE_TYPE(std::string_view)
    SPORE_META_DEFINE_TYPE(std::wstring)
    SPORE_META_DEFINE_TYPE(std::wstring_view)
    SPORE_META_DEFINE_TYPE(std::nullptr_t)
    SPORE_META_DEFINE_TYPE(std::byte)
    SPORE_META_DEFINE_TYPE(std::filesystem::path)

    SPORE_META_DEFINE_TEMPLATE_TYPE(std::allocator, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::array, typename, std::size_t)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::basic_string, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::basic_string_view, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::bitset, std::size_t)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::char_traits, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::deque, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::divides, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::equal_to, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::function, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::greater, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::greater_equal, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::hash, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::less, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::less_equal, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::list, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::map, typename, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::minus, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::multimap, typename, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::multiplies, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::multiset, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::not_equal_to, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::optional, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::pair, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::plus, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::priority_queue, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::queue, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::set, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::stack, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::unordered_map, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::unordered_set, typename, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::vector, typename, typename)

    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, typename, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, typename, typename, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, typename, typename, typename, typename, typename, typename, typename, typename)

    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, typename, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, typename, typename, typename, typename, typename, typename, typename)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, typename, typename, typename, typename, typename, typename, typename, typename)
}