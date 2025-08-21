#include "spore/examples/templates.hpp"

#include <iostream>

template <typename value_t>
struct storage
{
    alignas(value_t) std::byte storage[sizeof(value_t)];

    operator value_t&()
    {
        return *reinterpret_cast<value_t*>(&storage);
    }

    operator const value_t&() const
    {
        return *reinterpret_cast<const value_t*>(&storage);
    }
};

#include "spore/meta/extras/meta_extras_macros.hpp"

#define SPORE_META_REPEAT(Count, Macro, ...)
#define SPORE_META_REPEAT_1(Macro, ...) Macro(__VA_ARGS__)
#define SPORE_META_REPEAT_2(Macro, ...) Macro(__VA_ARGS__)

// #define SPORE_META_SUB_1(N)
// #define SPORE_META_SUB_2(N)
// #define SPORE_META_SUB_3(N)
// #define SPORE_META_SUB_4(N)
// #define SPORE_META_SUB_5(N)
// #define SPORE_META_SUB_6(N)
// #define SPORE_META_SUB_7(N)
// #define SPORE_META_SUB_8(N)

// #define SPORE_META_FOR_RANGE_N(Num, Macro, ...) SPORE_META_CONCAT(SPORE_META_FOR_, Num)(Macro, __VA_ARGS__)
// #define SPORE_META_FOR_RANGE(Macro, Min, Max) SPORE_META_FOR_N(SPORE_META_NUM_ARGS(__VA_ARGS__), Macro, __VA_ARGS__)
//
// #define SPORE_META_FOR_1(Macro, Value, ...) Macro(0, Value)
// #define SPORE_META_FOR_2(Macro, Value, ...) Macro(1, Value) SPORE_META_FOR_1(Macro, __VA_ARGS__)
// #define SPORE_META_FOR_3(Macro, Value, ...) Macro(2, Value) SPORE_META_FOR_2(Macro, __VA_ARGS__)
// #define SPORE_META_FOR_4(Macro, Value, ...) Macro(3, Value) SPORE_META_FOR_3(Macro, __VA_ARGS__)
// #define SPORE_META_FOR_5(Macro, Value, ...) Macro(4, Value) SPORE_META_FOR_4(Macro, __VA_ARGS__)
// #define SPORE_META_FOR_6(Macro, Value, ...) Macro(5, Value) SPORE_META_FOR_5(Macro, __VA_ARGS__)
// #define SPORE_META_FOR_7(Macro, Value, ...) Macro(6, Value) SPORE_META_FOR_6(Macro, __VA_ARGS__)
// #define SPORE_META_FOR_8(Macro, Value, ...) Macro(7, Value) SPORE_META_FOR_7(Macro, __VA_ARGS__)

#define SPORE_META_FUNCTION_DEFINE(Index, Value)

#define SPORE_META_FUNCTION_INNER_INNER_FOR(Index, Value)

#define SPORE_META_FUNCTION_INNER_FOR(Index, Value) \
    SPORE_META_FOR(SPORE_META_FUNCTION_INNER_INNER_FOR, 0, 1, 2, 3, 4, 5, 6, 7)

SPORE_META_FOR(SPORE_META_FUNCTION_INNER_FOR, 0, 1, 2, 3, 4, 5, 6, 7)

//#define SPORE_META_PARAM_TYPES(Macro, Value) \
//    Macro(Value, typename)                   \
//    Macro(Value, auto)
//
// #define SPORE_META_PARAM_MULTIPLICITY(Macro, Value) \
//    Macro(Value, 0)                                 \
//        Macro(Value, 0, 1)                          \
//            Macro(Value, 0, 1, 2)                   \
//                Macro(Value, 0, 1, 2, 3)            \
//                    Macro(Value, 0, 1, 2, 3, 4)
//
// #define WTF1(Value, Type) Type _t##Value SPORE_META_IF(SPORE_META_BOOL(Value), SPORE_META_COMMA, SPORE_META_EMPTY)()
// #define WTF2(Value, ...) SPORE_META_FOR(WTF1, )

#define SPORE_META_PARAM_TYPES(Macro, Value) \
    Macro(Value, typename)                   \
    Macro(Value, auto)

#define SPORE_META_PARAM_INDICES(Macro, Value) \
    Macro(Value, 0)                            \
        Macro(Value, 1)                        \
            Macro(Value, 2)                    \
                Macro(Value, 3)                \
                    Macro(Value, 4)

#define SPORE_META_PARAM_INDICES_(Value, Index) SPORE_META_PARAM_INDICES(SPORE_META_PARAM_TYPES, Index)

SPORE_META_PARAM_INDICES(SPORE_META_PARAM_INDICES, SPORE_META_PARAM_INDICES)

#define WTF1(Value, Type) Type _t##Value SPORE_META_IF(SPORE_META_BOOL(Value), SPORE_META_COMMA, SPORE_META_EMPTY)()
#define WTF2(Value, ...) SPORE_META_FOR(WTF1, )

int main()
{
    using namespace spore;
    using namespace spore::examples::templates;

    using value_t = templated_type<0, std::int32_t, std::int64_t, std::float_t>;

    // constexpr auto constructor = meta::find_constructor<value_t>([]<meta_constructor constructor_v> { return true; });
    constexpr auto function = meta::find_function<value_t>([]<meta_function function_v> { return true; });

    // storage<value_t> storage;
    // value_t value = constructor.construct(static_cast<std::int32_t>(0));
    value_t value;
    int result = function.invoke<int>(value, 3.14159f);

    return 0;
}