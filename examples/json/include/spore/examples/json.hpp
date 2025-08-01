#pragma once

#include "spore/meta.hpp"

#include "nlohmann/json.hpp"

#include SPORE_META_GENERATED("spore/examples/json.meta.hpp")

namespace spore::examples::json
{
    struct SPORE_META_TYPE() message
    {
        SPORE_META_FIELD(json = "identifier")
        int id = 0;

        SPORE_META_FIELD(json = false)
        int checksum = 0;

        SPORE_META_FIELD(json)
        std::string value;
    };

    template <typename value_t>
    void to_json(nlohmann::json& json, const value_t& value)
    {
        using namespace spore;

        const auto func = [&]<meta_field field_v> {
            constexpr auto predicate = []<meta_attribute attribute_v> { return attribute_v.name == "json"; };
            constexpr auto attribute = meta::find_attribute<field_v>(predicate);

            if constexpr (meta::is_valid(attribute))
            {
                if constexpr (meta::is_truthy(attribute.value))
                {
                    if constexpr (any_meta_string<decltype(attribute.value)>)
                    {
                        const std::string_view name = attribute.value.get();
                        json[name] = field_v.get(value);
                    }
                    else
                    {
                        const std::string_view name = field_v.name.get();
                        json[name] = field_v.get(value);
                    }
                }
            }
        };

        meta::for_each_field<value_t>(func);
    }

    template <typename value_t>
    void from_json(const nlohmann::json& json, value_t& value)
    {
        using namespace spore;

        const auto func = [&]<meta_field field_v> {
            constexpr auto predicate = []<meta_attribute attribute_v> { return attribute_v.name == "json"; };
            constexpr auto attribute = meta::find_attribute<field_v>(predicate);

            if constexpr (meta::is_valid(attribute))
            {
                if constexpr (meta::is_truthy(attribute.value))
                {
                    if constexpr (any_meta_string<decltype(attribute.value)>)
                    {
                        const std::string_view name = attribute.value.get();
                        json[name].get_to(field_v.get(value));
                    }
                    else
                    {
                        const std::string_view name = field_v.name.get();
                        json[name].get_to(field_v.get(value));
                    }
                }
            }
        };

        meta::for_each_field<value_t>(func);
    }
}

#include SPORE_META_GENERATED("spore/examples/json.meta.inl")