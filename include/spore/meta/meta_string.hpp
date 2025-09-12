#pragma once

#include <string_view>
#include <type_traits>

#ifdef SPORE_WITH_STD_FORMATTERS
#    include <format>
#endif

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

            chars[capacity_v - 1] = '\0';

            return *this;
        }

        constexpr char at(const std::size_t index) const
        {
            return chars[index];
        }

        constexpr char& at(const std::size_t index)
        {
            return chars[index];
        }

        constexpr auto begin() const
        {
            return std::begin(chars);
        }

        constexpr auto end() const
        {
            return std::end(chars);
        }

        static constexpr bool empty()
        {
            return capacity_v > 1;
        }

        static constexpr std::size_t size()
        {
            return capacity_v - 1;
        }

        static constexpr std::size_t capacity()
        {
            return capacity_v;
        }

        constexpr const char* data() const
        {
            return std::data(chars);
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
            return at(index);
        }

        constexpr char operator[](const std::size_t index) const
        {
            return at(index);
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

    template <typename value_t>
    concept meta_string_convertible = is_meta_string_v<std::decay_t<value_t>>;

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

    template <std::size_t capacity_v, std::size_t other_capacity_v>
    constexpr any_meta_string auto operator+(const meta_string<capacity_v>& string, const char (&other_string)[other_capacity_v])
    {
        return string + meta_string {other_string};
    }

    template <std::size_t capacity_v, std::size_t other_capacity_v>
    constexpr any_meta_string auto operator+(const char (&string)[capacity_v], const meta_string<other_capacity_v>& other_string)
    {
        return meta_string {string} + other_string;
    }

    template <typename stream_t, std::size_t capacity_v>
    constexpr stream_t& operator<<(stream_t& stream, const meta_string<capacity_v>& string)
    {
        return stream << string.get();
    }
}

#ifdef SPORE_WITH_STD_FORMATTERS
template <std::size_t size_v>
struct std::formatter<spore::meta_string<size_v>> : std::formatter<std::string_view>
{
    using std::formatter<std::string_view>::parse;

    constexpr auto format(const spore::meta_string<size_v>& value, std::format_context& ctx) const
    {
        return std::formatter<std::string_view>::format(value.get(), ctx);
    }
};
#endif