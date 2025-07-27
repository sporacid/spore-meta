#pragma once

#include <string_view>
#include <tuple>
#include <type_traits>

namespace spore
{
    template <std::size_t capacity_v>
    struct meta_string
    {
        static_assert(capacity_v > 0);

        char chars[capacity_v] {};

        constexpr meta_string() = default;

        constexpr meta_string(const char (&other_chars)[capacity_v])
        {
            *this = other_chars;
        }

        constexpr meta_string& operator=(const char (&other_chars)[capacity_v])
        {
            for (std::size_t index = 0; index < capacity_v; ++index)
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
            return capacity_v > 0 && chars[0] != '\0';
        }

        constexpr std::size_t size() const
        {
            return capacity_v - 1;
        }

        constexpr std::size_t capacity() const
        {
            return capacity_v;
        }

        constexpr const char* data() const
        {
            return std::data(chars);
        }

        template <std::size_t new_capacity_v>
        constexpr meta_string<new_capacity_v> resize() const
        {
            char new_chars[new_capacity_v];

            for (std::size_t index = 0; index < capacity_v && index < new_capacity_v; ++index)
            {
                new_chars[index] = chars[index];
            }

            new_chars[new_capacity_v - 1] = '\0';

            return meta_string<new_capacity_v> {new_chars};
        }

        constexpr std::string_view get() const
        {
            return std::string_view(chars);
        }

        constexpr operator std::string_view() const
        {
            return get();
        }

        constexpr char& operator[](const std::size_t index)
        {
            return chars[index];
        }

        constexpr char operator[](const std::size_t index) const
        {
            return chars[index];
        }

        constexpr bool operator==(const meta_string& other) const
        {
            return get() == other.get();
        }

        constexpr bool operator==(const std::string_view& other) const
        {
            return get() == other;
        }

        constexpr bool operator==(const char* other) const
        {
            return get() == std::string_view(other);
        }

        constexpr bool operator<(const meta_string& other) const
        {
            return get() < other.get();
        }

        constexpr bool operator<(const std::string_view& other) const
        {
            return get() < other;
        }

        constexpr bool operator<(const char* other) const
        {
            return get() < std::string_view(other);
        }
    };

    template <std::size_t capacity_v>
    meta_string(const char (&string)[capacity_v])
        -> meta_string<capacity_v>;

    template <typename>
    struct is_meta_string : std::false_type
    {
    };

    template <std::size_t capacity_v>
    struct is_meta_string<meta_string<capacity_v>> : std::true_type
    {
    };

    template <typename value_t>
    constexpr bool is_meta_string_v = is_meta_string<value_t>::value;

    template <typename value_t>
    concept any_meta_string = is_meta_string_v<value_t>;

    template <std::size_t capacity_v, std::size_t other_capacity_v>
    constexpr any_meta_string auto operator+(const meta_string<capacity_v>& string, const meta_string<other_capacity_v>& other_string)
    {
        meta_string<capacity_v + other_capacity_v - 1> new_string;

        for (std::size_t index = 0; index < string.size(); ++index)
        {
            new_string[index] = string[index];
        }

        for (std::size_t index = 0; index < other_string.size(); ++index)
        {
            new_string[index + string.size()] = other_string[index];
        }

        new_string[capacity_v + other_capacity_v - 2] = '\0';

        return new_string;
    }

    template <typename stream_t, std::size_t capacity_v>
    constexpr stream_t& operator<<(stream_t& stream, const meta_string<capacity_v>& string)
    {
        return stream << string.get();
    }

    namespace meta::strings
    {
        namespace detail
        {
            template <std::size_t index_v, std::size_t offset_v, std::size_t capacity_v, std::size_t... other_capacities_v>
            constexpr void concat_impl(meta_string<capacity_v>& string, const meta_string<other_capacities_v>&... other_strings)
            {
                if constexpr (index_v < sizeof...(other_strings))
                {
                    std::tuple tuple = std::tie(other_strings...);

                    const any_meta_string auto& other_string = std::get<index_v>(tuple);

                    constexpr std::array other_sizes {(other_capacities_v - 1)...};

                    for (std::size_t index = 0; index < other_sizes[index_v]; ++index)
                    {
                        string.chars[offset_v + index] = other_string.chars[index];
                    }

                    concat_impl<index_v + 1, offset_v + other_sizes[index_v]>(string, other_strings...);
                }
            }
        }

        template <std::size_t... capacities_v>
        constexpr any_meta_string auto concat(const meta_string<capacities_v>&... other_strings)
        {
            constexpr std::size_t size = (1ULL + ... + (capacities_v - 1));

            meta_string<size> string;

            detail::concat_impl<0, 0>(string, other_strings...);

            return string;
        }
    }
}