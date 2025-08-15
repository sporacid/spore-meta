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

int main()
{
    using namespace spore;
    using namespace spore::examples::templates;

    using value_t = templated_type<0, std::int32_t, std::int64_t, std::float_t>;

    constexpr auto constructor = meta::find_constructor<value_t>([]<meta_constructor constructor_v> { return true; });
    constexpr auto function = meta::find_function<value_t>([]<meta_function function_v> { return true; });

    // storage<value_t> storage;
    value_t value = constructor.construct(static_cast<std::int32_t>(0));
    int result = function.invoke<int>(value, 3.14159f);

    return 0;
}