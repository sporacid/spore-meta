#pragma once

#ifndef SPORE_CODEGEN
#    define SPORE_META_ATTRIBUTE(...)
#    define SPORE_META_GENERATED(File) File
#else
#    define SPORE_META_ATTRIBUTE(...) [[clang::annotate(#__VA_ARGS__)]]
#    define SPORE_META_GENERATED(File) "spore/meta/meta_codegen.hpp"
#endif

#define SPORE_META_TYPE(...) SPORE_META_ATTRIBUTE(_spore_meta_enabled, __VA_ARGS__)
#define SPORE_META_ENUM(...) SPORE_META_ATTRIBUTE(_spore_meta_enabled, __VA_ARGS__)
#define SPORE_META_ENUM_VALUE(...) SPORE_META_ATTRIBUTE(_spore_meta_enabled, __VA_ARGS__)
#define SPORE_META_FIELD(...) SPORE_META_ATTRIBUTE(_spore_meta_enabled, __VA_ARGS__)
#define SPORE_META_FUNCTION(...) SPORE_META_ATTRIBUTE(_spore_meta_enabled, __VA_ARGS__)
#define SPORE_META_CONSTRUCTOR(...) SPORE_META_ATTRIBUTE(_spore_meta_enabled, __VA_ARGS__)
