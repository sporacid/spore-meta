#include "spore/examples/extensions.hpp"

#include <iostream>

int main()
{
    using namespace spore;
    using namespace spore::examples::extensions;

    constexpr auto predicate = []<any_meta_extension auto extension_v> { return std::same_as<extension, decltype(extension_v)>; };
    constexpr auto extension = meta::find_extension<widget>(predicate);

    widget widget;
    extension.draw_widget(&widget);

    return 0;
}