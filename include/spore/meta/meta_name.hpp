// #pragma once
//
// #include <string>
// #include <string_view>
// #include <type_traits>
//
// namespace spore::meta
// {
//     namespace detail
//     {
//         template <typename value_t>
//         struct meta_name_impl;
//
//         template <>
//         struct meta_name_impl<void>
//         {
//             static consteval std::string get()
//             {
//                 return "void";
//             }
//         };
//
//         template <typename value_t>
//         requires(std::is_const_v<value_t> or std::is_volatile_v<value_t>) struct meta_name_impl<value_t>
//         {
//             static consteval std::string get()
//             {
//                 std::string prefix;
//
//                 if constexpr (std::is_const_v<value_t>)
//                 {
//                     prefix += "const ";
//                 }
//
//                 if constexpr (std::is_volatile_v<value_t>)
//                 {
//                     prefix += "volatile ";
//                 }
//
//                 return prefix + meta_name_impl<std::remove_cv_t<value_t>>::get();
//             }
//         };
//
//         template <typename value_t>
//         requires std::is_lvalue_reference_v<value_t> struct meta_name_impl<value_t>
//         {
//             static consteval std::string get()
//             {
//                 return meta_name_impl<std::remove_reference_t<value_t>>::get() + "&";
//             }
//         };
//
//         template <typename value_t>
//         requires std::is_rvalue_reference_v<value_t> struct meta_name_impl<value_t>
//         {
//             static consteval std::string get()
//             {
//                 return meta_name_impl<std::remove_reference_t<value_t>>::get() + "&&";
//             }
//         };
//
//         template <typename value_t>
//         requires std::is_pointer_v<value_t> struct meta_name_impl<value_t>
//         {
//             static consteval std::string get()
//             {
//                 return meta_name_impl<std::remove_pointer_t<value_t>>::get() + "*";
//             }
//         };
//
//         template <typename value_t, std::size_t size_v>
//         struct meta_name_impl<value_t[size_v]>
//         {
//             static consteval std::string get()
//             {
//                 return meta_name_impl<value_t>::get() + "[" + std::to_string(size_v) + "]";
//             }
//         };
//
//         template <typename value_t>
//         struct meta_name_impl<value_t[]>
//         {
//             static consteval std::string value()
//             {
//                 return meta_name_impl<value_t>::get() + "[]";
//             }
//         };
//
//         template <typename result_t, typename... args_t>
//         struct meta_name_impl<result_t(args_t...)>
//         {
//             static consteval std::string value()
//             {
//                 constexpr std::string_view separator = ", ";
//                 constexpr std::array args {meta_name_impl<args_t>::get()...};
//
//                 std::string string = meta_name_impl<result_t>::get() + "(";
//
//                 for (const std::string_view& arg : args)
//                 {
//                     string += arg;
//                     string += separator;
//                 }
//
//                 if (string.size() >= separator.size())
//                 {
//                     string.resize(string.size() - separator.size());
//                 }
//
//                 string += ")";
//
//                 return string;
//             }
//         };
//
//         template <meta_enabled value_t>
//         struct meta_name_impl<value_t>
//         {
//             static std::string value()
//             {
//                 constexpr meta_type type = get_type<value_t>();
//                 return type.name;
//             }
//         };
// #if 0
//         template <>
//         struct meta_name_impl<void>
//         {
//             static std::string value()
//             {
//                 return "void";
//             }
//         };
//
//         template <typename value_t>
//         struct get_name_impl<value_t, std::enable_if_t<std::is_const_v<value_t> || std::is_volatile_v<value_t>>>
//         {
//             static std::string value()
//             {
//                 std::string prefix;
//
//                 if constexpr (std::is_const_v<value_t>)
//                 {
//                     prefix += "const ";
//                 }
//
//                 if constexpr (std::is_volatile_v<value_t>)
//                 {
//                     prefix += "volatile ";
//                 }
//
//                 return prefix + get_name_impl<std::remove_const_t<value_t>>::value();
//             }
//         };
//
//         template <typename value_t>
//         struct get_name_impl<value_t, std::enable_if_t<std::is_lvalue_reference_v<value_t>>>
//         {
//             static std::string value()
//             {
//                 return get_name_impl<std::remove_reference_t<value_t>>::value() + "&";
//             }
//         };
//
//         template <typename value_t>
//         struct get_name_impl<value_t, std::enable_if_t<std::is_rvalue_reference_v<value_t>>>
//         {
//             static std::string value()
//             {
//                 return get_name_impl<std::remove_reference_t<value_t>>::value() + "&&";
//             }
//         };
//
//         template <typename value_t>
//         struct get_name_impl<value_t, std::enable_if_t<std::is_pointer_v<value_t>>>
//         {
//             static std::string value()
//             {
//                 return get_name_impl<std::remove_pointer_t<value_t>>::value() + "*";
//             }
//         };
//
//         template <typename value_t, std::size_t size_v>
//         struct get_name_impl<value_t[size_v]>
//         {
//             static std::string value()
//             {
//                 return get_name_impl<value_t>::value() + "[" + std::to_string(size_v) + "]";
//             }
//         };
//
//         template <typename value_t>
//         struct get_name_impl<value_t[]>
//         {
//             static std::string value()
//             {
//                 return get_name_impl<value_t>::value() + "[]";
//             }
//         };
//
//         template <typename result_t, typename... args_t>
//         struct get_name_impl<result_t(args_t...)>
//         {
//             static std::string value()
//             {
//                 std::array<std::string, sizeof...(args_t)> args {
//                     get_name_impl<args_t>::value()...,
//                 };
//
//                 return get_name_impl<result_t>::value() + "(" + strings::join(args, ", ", std::identity()) + ")";
//             }
//         };
//
//         template <meta_type value_t>
//         struct get_name_impl<value_t>
//         {
//             static std::string value()
//             {
//                 return meta_traits<value_t>::get_name();
//             }
//         };
// #endif
//     }
//
// }