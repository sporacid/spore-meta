#pragma once

namespace spore
{
    enum class meta_result
    {
        continue_,
        break_,
    };

    namespace meta
    {
        constexpr meta_result continue_ = meta_result::continue_;
        constexpr meta_result break_ = meta_result::break_;
    }
}