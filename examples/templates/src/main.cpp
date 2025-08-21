#include "spore/examples/templates.hpp"

#include <iostream>

int main()
{
    using namespace spore;
    using namespace spore::examples::templates;

    using value_t = templated_type<0, std::int32_t, std::int64_t, std::float_t>;

    constexpr auto constructor = meta::find_constructor<value_t>([]<meta_constructor constructor_v> { return true; });
    constexpr auto function = meta::find_function<value_t>([]<meta_function function_v> { return true; });

    const value_t value = constructor.construct(1, 2, 3);
    const int result = function.invoke<int>(value, 3.14159f);

    std::cout << "Templated function returned " << std::to_string(result) << std::endl;

    return 0;
}