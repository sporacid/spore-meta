#include "spore/examples/extensions.hpp"

int main()
{
    using namespace spore;
    using namespace spore::examples::extensions;

    constexpr auto value_predicate = []<any_meta_extension auto extension_v> { return std::same_as<value_extension<value>, decltype(extension_v)>; };
    constexpr auto value_extension = meta::find_extension<value>(value_predicate);

    value value;
    std::ignore = value_extension.hash_value(value);

    constexpr auto widget_predicate = []<any_meta_extension auto extension_v> { return std::same_as<widget_extension<widget>, decltype(extension_v)>; };
    constexpr auto widget_extension = meta::find_extension<widget>(widget_predicate);

    widget widget;
    widget_extension.draw_widget(widget);

    return 0;
}