#pragma once

#include "spore/meta/meta_accessors.hpp"
#include "spore/meta/meta_adl.hpp"
#include "spore/meta/meta_type_id.hpp"

#include <functional>
#include <mutex>
#include <unordered_set>
#include <vector>

namespace spore
{
    struct meta_type_registrar
    {
        template <typename value_t>
        void add_or_register_type(const meta_adl<value_t>)
        {
            constexpr bool has_register_type = requires {
                { register_type(meta_adl<value_t> {}) } -> std::same_as<void>;
            };

            if constexpr (has_register_type)
            {
                std::lock_guard lock {mutex};

                constexpr meta_type_id type_id = meta::get_id<value_t>();

                if (not registered_types.contains(type_id))
                {
                    auto register_func = [] { register_type(meta_adl<value_t> {}); };

                    if (is_pre_registration)
                    {
                        register_funcs.emplace_back(std::move(register_func));
                    }
                    else
                    {
                        register_func();
                    }

                    registered_types.emplace(type_id);
                }
            }
        }

        void register_types()
        {
            std::lock_guard lock {mutex};

            if (not register_funcs.empty())
            {
                for (std::size_t index = 0; index < register_funcs.size(); ++index)
                {
                    register_funcs[index]();
                }

                register_funcs.clear();
                register_funcs.shrink_to_fit();
            }

            is_pre_registration = false;
        }

        static meta_type_registrar& get()
        {
            static meta_type_registrar instance;
            return instance;
        }

      private:
        meta_type_registrar() = default;

        std::recursive_mutex mutex;
        std::vector<std::function<void()>> register_funcs;
        std::unordered_set<meta_type_id> registered_types;
        bool is_pre_registration = true;
    };

    template <typename value_t>
    struct meta_register_type_guard
    {
        meta_register_type_guard()
        {
            meta_type_registrar::get().add_or_register_type(meta_adl<value_t> {});
        }
    };

    namespace meta
    {
        template <typename value_t>
        void register_type()
        {
            meta_type_registrar::get().add_or_register_type(meta_adl<value_t> {});
        }

        inline void register_types()
        {
            meta_type_registrar::get().register_types();
        }

        template <typename value_t>
        const meta_register_type_guard<value_t> register_type_guard;
    }
}