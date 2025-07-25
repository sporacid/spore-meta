#pragma once

#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_type.hpp"

#include <cstdint>
#include <string>
#include <string_view>

#define SPORE_META_DEFINE_STD_TYPE(Name)                                \
    template <typename func_t>                                          \
    constexpr auto with_meta_type(spore::meta_adl<Name>, func_t&& func) \
    {                                                                   \
        constexpr spore::meta_type type {                               \
            .name = #Name,                                              \
            .bases = meta_tuple {},                                     \
            .fields = meta_tuple {},                                    \
            .functions = meta_tuple {},                                 \
            .constructors = meta_tuple {},                              \
            .attributes = meta_tuple {},                                \
        };                                                              \
                                                                        \
        return func.template operator()<type>();                        \
    }

#define SPORE_META_DEFINE_STD_TEMPLATE(Name, ...)

namespace spore
{
    SPORE_META_DEFINE_STD_TYPE(char);
    SPORE_META_DEFINE_STD_TYPE(wchar_t);
    SPORE_META_DEFINE_STD_TYPE(std::int8_t);
    SPORE_META_DEFINE_STD_TYPE(std::int16_t);
    SPORE_META_DEFINE_STD_TYPE(std::int32_t);
    SPORE_META_DEFINE_STD_TYPE(std::int64_t);
    SPORE_META_DEFINE_STD_TYPE(std::uint8_t);
    SPORE_META_DEFINE_STD_TYPE(std::uint16_t);
    SPORE_META_DEFINE_STD_TYPE(std::uint32_t);
    SPORE_META_DEFINE_STD_TYPE(std::uint64_t);
    SPORE_META_DEFINE_STD_TYPE(std::float_t);
    SPORE_META_DEFINE_STD_TYPE(std::double_t);
    SPORE_META_DEFINE_STD_TYPE(std::string);
    SPORE_META_DEFINE_STD_TYPE(std::string_view);

    template <typename value_t, typename func_t>
    constexpr auto with_meta_type(meta_adl<std::allocator<value_t>>, func_t&& func)
    {
        constexpr meta_type type {
            .name = "std::allocator",
            .bases = meta_tuple {},
            .fields = meta_tuple {},
            .functions = meta_tuple {},
            .constructors = meta_tuple {},
            .attributes = meta_tuple {},
        };

        return func.template operator()<type>();
    }

    template <typename value_t, typename allocator_t, typename func_t>
    constexpr auto with_meta_type(meta_adl<std::vector<value_t, allocator_t>>, func_t&& func)
    {
        constexpr meta_type type {
            .name = "std::vector",
            .bases = meta_tuple {},
            .fields = meta_tuple {},
            .functions = meta_tuple {},
            .constructors = meta_tuple {},
            .attributes = meta_tuple {},
        };

        return func.template operator()<type>();
    }
}