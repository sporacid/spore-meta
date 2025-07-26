#pragma once

#include <string_view>
#include <tuple>
#include <type_traits>

namespace spore
{
    template <std::size_t size_v>
    struct meta_string
    {
        char chars[size_v] {};

        constexpr meta_string() = default;

        constexpr meta_string(const char (&other_chars)[size_v])
        {
            *this = other_chars;
        }

        constexpr meta_string& operator=(const char (&other_chars)[size_v])
        {
            for (std::size_t index = 0; index < size_v; ++index)
            {
                chars[index] = other_chars[index];
            }

            return *this;
        }

        constexpr auto begin() const
        {
            return std::begin(chars);
        }

        constexpr auto end() const
        {
            return std::end(chars);
        }

        constexpr bool empty() const
        {
            return size_v > 1;
        }

        constexpr std::size_t size() const
        {
            return size_v - 1;
        }

        constexpr std::size_t capacity() const
        {
            return size_v;
        }

        constexpr const char* data() const
        {
            return std::data(chars);
        }

        template <std::size_t new_size_v>
        constexpr meta_string<new_size_v> resize() const
        {
            char new_chars[new_size_v];

            for (std::size_t index = 0; index < size_v && index < new_size_v; ++index)
            {
                new_chars[index] = chars[index];
            }

            new_chars[new_size_v - 1] = '\0';

            return meta_string<new_size_v> {new_chars};
        }

        constexpr std::string_view get() const
        {
            return std::string_view(chars, size_v - 1);
        }

        constexpr operator std::string_view() const
        {
            return get();
        }

        constexpr char operator[](const std::size_t index) const
        {
            return chars[index];
        }

        template <std::size_t other_size_v>
        constexpr bool operator==(const meta_string<other_size_v>& other) const
        {
            return size_v == other_size_v && get() == other.get();
        }

        constexpr bool operator==(const std::string_view& other) const
        {
            return get() == other;
        }

        template <std::size_t other_size_v>
        constexpr bool operator<(const meta_string<other_size_v>& other) const
        {
            return size_v < other_size_v or get() < other.get();
        }

        constexpr bool operator<(const std::string_view& other) const
        {
            return get() < other;
        }
    };

    template <std::size_t size_v>
    meta_string(const char (&string)[size_v])
        -> meta_string<size_v>;

    template <typename stream_t, std::size_t size_v>
    constexpr stream_t& operator<<(stream_t& stream, const meta_string<size_v>& string)
    {
        return stream << string.get();
    }

    template <typename>
    struct is_meta_string : std::false_type
    {
    };

    template <std::size_t size_v>
    struct is_meta_string<meta_string<size_v>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_string_v = is_meta_string<value_t>::value;

    template <typename value_t>
    concept any_meta_string = is_meta_string_v<value_t>;

    namespace meta::strings
    {
        namespace detail
        {
            template <std::size_t index_v, std::size_t offset_v, std::size_t size_v, std::size_t... other_sizes_v>
            constexpr void concat_impl(meta_string<size_v>& string, const meta_string<other_sizes_v>&... other_strings)
            {
                if constexpr (index_v < sizeof...(other_strings))
                {
                    std::tuple tuple = std::tie(other_strings...);

                    const any_meta_string auto& other_string = std::get<index_v>(tuple);

                    constexpr std::array other_sizes {(other_sizes_v - 1)...};

                    for (std::size_t index = 0; index < other_sizes[index_v]; ++index)
                    {
                        string.chars[offset_v + index] = other_string.chars[index];
                    }

                    concat_impl<index_v + 1, offset_v + other_sizes[index_v]>(string, other_strings...);
                }
            }
        }

        template <std::size_t... sizes_v>
        constexpr any_meta_string auto concat(const meta_string<sizes_v>&... other_strings)
        {
            constexpr std::size_t size = (1ULL + ... + (sizes_v - 1));

            meta_string<size> string;

            detail::concat_impl<0, 0>(string, other_strings...);

            return string;
        }

        template <std::size_t new_size_v, std::size_t size_v>
        constexpr any_meta_string auto resize(const meta_string<size_v>& string)
        {
            meta_string<new_size_v> new_string {string};
            return new_string;
        }
    }
}