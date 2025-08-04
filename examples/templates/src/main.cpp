#include "spore/examples/templates.hpp"

#include <iostream>

int main()
{
    using namespace spore;
    using namespace spore::examples::templates;

    using value_t = templated_type<0, std::int32_t, std::int64_t, std::float_t>;

    constexpr auto predicate = []<meta_constructor constructor_v> { return true; };
    constexpr auto constructor = meta::find_constructor<value_t>(predicate);

    alignas(value_t) std::byte storage[sizeof(value_t)];

    return 0;
}