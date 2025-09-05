#pragma once

#include "spore/meta/meta_adl.hpp"

#include <functional>
#include <mutex>
#include <ranges>
#include <vector>

namespace spore
{
    namespace meta
    {
        template <typename value_t, typename void_t>
        void register_type(const meta_adl<value_t>, const meta_adl<void_t>)
        {
        }

        template <typename value_t>
        void register_type(const meta_adl<value_t>)
        {
            register_type(meta_adl<value_t> {}, meta_adl<void> {});
        }
    }

    namespace detail
    {
        struct meta_register_types
        {
            template <typename value_t>
            static void add_or_register_type(const meta_adl<value_t>)
            {
                std::lock_guard lock {mutex};

                auto register_func = [] {
                    using namespace spore::meta;
                    register_type(meta_adl<value_t> {});
                };

                if (is_pre_registration)
                {
                    register_funcs.emplace_back(std::move(register_func));
                }
                else
                {
                    register_func();
                }
            }

            static void register_types()
            {
                std::lock_guard lock {mutex};

                if (not register_funcs.empty())
                {
                    std::ranges::for_each(register_funcs, &std::function<void()>::operator());
                    register_funcs.clear();
                    register_funcs.shrink_to_fit();
                }

                is_pre_registration = false;
            }

          private:
            static inline std::recursive_mutex mutex;
            static inline std::vector<std::function<void()>> register_funcs;
            static inline bool is_pre_registration = true;
        };
    }

    template <typename value_t>
    struct meta_register_type_guard
    {
        meta_register_type_guard()
        {
            detail::meta_register_types::add_or_register_type(meta_adl<value_t> {});
        }
    };

    namespace meta
    {
        template <typename value_t>
        void register_type()
        {
            detail::meta_register_types::add_or_register_type(meta_adl<value_t> {});
        }

        inline void register_types()
        {
            detail::meta_register_types::register_types();
        }

        template <typename value_t>
        const meta_register_type_guard<value_t> register_type_guard;
    }
}