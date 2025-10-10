#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_enabled.hpp"
#include "spore/meta/meta_string_convert.hpp"
#include "spore/meta/meta_type.hpp"

#include "spore/meta/extras/meta_extras_macros.hpp"

#ifdef SPORE_WITH_TYPE_REGISTRATION
#    include "spore/meta/extras/meta_extra_type_registration.hpp"
#endif

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
#include <ranges>
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
    SPORE_META_DEFINE_TYPE(bool)

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

    SPORE_META_DEFINE_TEMPLATE_TYPE(std::allocator, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::array, meta_enabled, std::size_t)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::basic_string, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::basic_string_view, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::bitset, std::size_t)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::char_traits, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::deque, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::divides, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::equal_to, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::function, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::greater, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::greater_equal, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::hash, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::less, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::less_equal, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::list, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::map, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::minus, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::multimap, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::multiplies, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::multiset, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::not_equal_to, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::optional, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::pair, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::plus, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::priority_queue, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::queue, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::reference_wrapper, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::set, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::shared_ptr, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::span, meta_enabled, std::size_t)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::stack, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::unique_ptr, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::unordered_map, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::unordered_multimap, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::unordered_multiset, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::unordered_set, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::vector, meta_enabled, meta_enabled)

    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::tuple, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled)

    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
    SPORE_META_DEFINE_TEMPLATE_TYPE(std::variant, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled, meta_enabled)
}