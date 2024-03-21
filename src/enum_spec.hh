#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <format>
#include <iostream>
#include <string>
#include <string_view>

template<typename En>
struct enum_spec : std::false_type{};

template <typename En>
concept string_enum =
    std::same_as<decltype(enum_spec<En>::name), const std::string_view> &&
    std::same_as<decltype(enum_spec<En>::members),
                 const std::array<std::pair<std::string_view, En>,
                                  enum_spec<En>::n_members>>;

template<string_enum En>
static inline constexpr std::string_view to_string(En value)
{
    constexpr auto &m = enum_spec<En>::members;

    const auto it = std::find_if(m.begin(), m.end(), [&value](const auto &v) { return v.second == value; });

    if (it == m.end()) {
        throw std::range_error(std::format("out of range value for {}", enum_spec<En>::name));
    }
    return it->first;
}

template<string_enum En>
static inline constexpr En from_string(std::string_view s)
{
    constexpr auto &m = enum_spec<En>::members;

    const auto it = std::find_if(m.begin(), m.end(), [&s](const auto &v) { return v.first == s; });

    if (it == m.end()) {
        throw std::range_error(std::format("could not read {} from string", std::string(enum_spec<En>::name)));
    }
    return it->second;
}

template<string_enum T>
static std::ostream inline &operator<<(std::ostream& os, const T& e)
{
    return os << to_string<T>(e);
}

template<string_enum T>
static std::istream inline &operator>>(std::istream& is, T& e)
{
    std::string s;
    is >> s;

    e = from_string<T>(s);

    return is;
}
