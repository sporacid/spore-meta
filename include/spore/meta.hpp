#pragma once

#include "spore/meta/meta.hpp"
#include "spore/meta/meta_constant.hpp"
#include "spore/meta/meta_macros.hpp"
#include "spore/meta/meta_string_convert.hpp"
#include "spore/meta/meta_truthy.hpp"
#include "spore/meta/meta_version.hpp"

#ifdef SPORE_WITH_STD_TYPES
#    include "spore/meta/extras/meta_extras_std_types.hpp"
#endif

#ifdef SPORE_WITH_TYPE_REGISTRATION
#    include "spore/meta/extras/meta_extra_type_registration.hpp"
#endif