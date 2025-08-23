#pragma once

#define SPORE_META_NUM_ARGS(...) SPORE_META_NUM_ARGS_(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define SPORE_META_NUM_ARGS_(_8, _7, _6, _5, _4, _3, _2, _1, N, ...) N

#define SPORE_META_CONCAT(A, B) SPORE_META_CONCAT_(A, B)
#define SPORE_META_CONCAT_(A, B) A##B

#define SPORE_META_FOR_N(Num, Macro, ...) SPORE_META_CONCAT(SPORE_META_FOR_, Num)(Macro, __VA_ARGS__)
#define SPORE_META_FOR(Macro, ...) SPORE_META_FOR_N(SPORE_META_NUM_ARGS(__VA_ARGS__), Macro, __VA_ARGS__)

#define SPORE_META_FOR_1(Macro, Value, ...) Macro(0, Value)
#define SPORE_META_FOR_2(Macro, Value, ...) Macro(1, Value) SPORE_META_FOR_1(Macro, __VA_ARGS__)
#define SPORE_META_FOR_3(Macro, Value, ...) Macro(2, Value) SPORE_META_FOR_2(Macro, __VA_ARGS__)
#define SPORE_META_FOR_4(Macro, Value, ...) Macro(3, Value) SPORE_META_FOR_3(Macro, __VA_ARGS__)
#define SPORE_META_FOR_5(Macro, Value, ...) Macro(4, Value) SPORE_META_FOR_4(Macro, __VA_ARGS__)
#define SPORE_META_FOR_6(Macro, Value, ...) Macro(5, Value) SPORE_META_FOR_5(Macro, __VA_ARGS__)
#define SPORE_META_FOR_7(Macro, Value, ...) Macro(6, Value) SPORE_META_FOR_6(Macro, __VA_ARGS__)
#define SPORE_META_FOR_8(Macro, Value, ...) Macro(7, Value) SPORE_META_FOR_7(Macro, __VA_ARGS__)

#define SPORE_META_BOOL(Value) SPORE_META_CONCAT(SPORE_META_BOOL_, Value)
#define SPORE_META_BOOL_0 0
#define SPORE_META_BOOL_1 1
#define SPORE_META_BOOL_2 1
#define SPORE_META_BOOL_3 1
#define SPORE_META_BOOL_4 1
#define SPORE_META_BOOL_5 1
#define SPORE_META_BOOL_6 1
#define SPORE_META_BOOL_7 1

#define SPORE_META_IF_0(True, False) False
#define SPORE_META_IF_1(True, False) True

#define SPORE_META_IF_(Bool, True, False) SPORE_META_CONCAT(SPORE_META_IF_, Bool)(True, False)
#define SPORE_META_IF(Condition, True, False) SPORE_META_IF_(SPORE_META_CONCAT(SPORE_META_BOOL_, Condition), True, False)

#define SPORE_META_EMPTY()
#define SPORE_META_COMMA() ,

#define SPORE_META_TEMPLATE_PARAM(Index, Value) Value _t##Index,
#define SPORE_META_TEMPLATE_PARAM_NAME(Index, Value) _t##Index SPORE_META_IF(Index, SPORE_META_COMMA, SPORE_META_EMPTY)()
#define SPORE_META_TEMPLATE_PARAM_STRING(Index, Value) +spore::meta::strings::to_string<_t##Index>() SPORE_META_IF(Index, +", ", )

#define SPORE_META_DEFINE_TYPE(Name)                                    \
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
            .extensions = meta_tuple {},                                \
        };                                                              \
                                                                        \
        return func.template operator()<type>();                        \
    }

#define SPORE_META_DEFINE_TEMPLATE_TYPE(Name, ...)                                                                            \
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
            .extensions = meta_tuple {},                                                                                      \
        };                                                                                                                    \
                                                                                                                              \
        return func.template operator()<type>();                                                                              \
    }