#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <string_view>

namespace spore::meta::codegen::utils
{
    namespace detail
    {
        constexpr std::size_t size(char const&)
        {
            return 1U;
        }

        constexpr const char* data(char const& c)
        {
            return &c;
        }

        template <std::size_t size_v>
        constexpr std::size_t size(char const (&)[size_v])
        {
            return size_v - 1;
        }

        constexpr char* copy_n(const char* source, const std::size_t size, char* dest)
        {
            for (std::size_t index = 0; index < size; ++index)
            {
                *(dest + index) = *(source + index);
            }

            return dest + size;
        }

        template <typename... chars_t>
        constexpr auto cat(chars_t const&... cs)
        {
            using std::data;

            std::array<char, (0U + ... + size(cs))> ret {{}};

            if constexpr (ret.size())
            {
                char* p = ret.data();
                ((p = copy_n(data(cs), size(cs), p)), ...);
            }

            return ret;
        }
    }

#if 0
    namespace detail
    {
#    if 0
        template <bool value_v>
        consteval std::string to_string_impl()
        {
            return value_v ? std::string {"true"} : std::string {"false"};
        }

        template <auto value_v>
        consteval std::string to_string_impl() requires std::integral<decltype(value_v)>
        {
            std::string string;

            std::integral auto value_copy = value_v;

            std::size_t index = 0;

            if (value_copy == 0)
            {
                string.push_back('0');
            }
            else if (value_copy < 0)
            {
                string.push_back('-');
                index = 1;
                value_copy *= -1;
            }

            while (value_copy > 0)
            {
                const char c = '0' + value_copy % 10;
                string.push_back(c);
                value_copy /= 10;
                ++index;
            }

            return string;
        }
#    else
        consteval std::string to_string_impl(const bool value)
        {
            return value ? std::string {"true"} : std::string {"false"};
        }

        template <std::integral value_t>
        consteval std::string to_string_impl(const value_t value)
        {
            std::string string;

            value_t value_copy = value;

            std::size_t index = 0;

            if (value_copy == 0)
            {
                string.push_back('0');
            }
            else if (value_copy < 0)
            {
                string.push_back('-');
                index = 1;
                value_copy *= -1;
            }

            while (value_copy > 0)
            {
                const char c = '0' + value_copy % 10;
                string.push_back(c);
                value_copy /= 10;
                ++index;
            }

            return string;
        }
#    endif

        template <std::size_t size_v>
        consteval void to_char_array(const std::string& string, std::array<char, size_v>& array)
        {
            for (std::size_t index = 0; index < size_v && index < string.size() - 1; ++index)
            {
                array[index] = string[index];
            }

            array[size_v - 1] = '\0';
        }
    }

    template <auto value_v>
    consteval std::string_view to_string()
    {
        constexpr std::array array = [] {
            constexpr std::string string = detail::to_string_impl(value_v);
            constexpr std::size_t array_size = string.size() + 1;
            std::array<char, array_size> array;
            detail::to_char_array(string, array);
            return array;
        }();
        return array.data();
    }

    consteval const char* adafaaffa()
    {
        constexpr char value[13] {};
        return value;
    }
#endif
}