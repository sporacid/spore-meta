#pragma once

#include "spore/meta/meta.hpp"

#include <map>
#include <mutex>

namespace spore::examples::registration
{
    struct registry
    {
        template <typename value_t>
        void add_type()
        {
            std::lock_guard lock {_mutex};

            _type_map.emplace(meta::get_id<value_t>(), meta::get_name<value_t>());
        }

        template <typename func_t>
        void for_each_type(func_t&& func)
        {
            std::lock_guard lock {_mutex};

            for (const auto& [type_id, type_name] : _type_map)
            {
                func(type_id, type_name);
            }
        }

        static registry& instance()
        {
            static registry instance;
            return instance;
        }

      private:
        std::mutex _mutex;
        std::map<meta_type_id, std::string> _type_map;
    };

    template <typename value_t>
    void register_type(const meta_adl<value_t>, const meta_adl<void>)
    {
        registry::instance().add_type<value_t>();

        constexpr auto action = []<meta_field field_v> {
            using field_t = std::decay_t<typename decltype(field_v)::value_type>;
            registry::instance().add_type<field_t>();
        };

        meta::for_each_field<value_t>(action);
    }
}