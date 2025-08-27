# What is Spore Meta

`spore-meta` is a C++20, header-only library to define compile-time reflection metadata for any given type. The library
is optionally integrated with [spore-codegen](https://github.com/sporacid/spore-codegen) to automatically generate the
reflection
metadata via `libclang` and with CMake to run the code generation automatically when building a target.

## Table of Contents

- [Quick Start](#quick-start)
- [Integrations](#integrations)
    - [CMake and Spore Codegen](#cmake-and-spore-codegen)
        - [Explicit Code Generation](#explicit-code-generation)
        - [Implicit Code Generation](#implicit-code-generation)
        - [Custom Code Generation](#custom-code-generation)
    - [Vcpkg](#vcpkg)
- [Query Interface](#query-interface)
    - [For Each](#for-each)
    - [Find](#find)
    - [Attributes](#attributes)
- [Examples](#examples)
    - [Hello World](#hello-world)
    - [Implicit Code Generation](#implicit-code-generation)
    - [Json Serialization](#json-serialization)
    - [External Project Integration](#external-project-integration)

# Quick Start

If you're only interested in the reflection structure, you can copy the `include` directory in your project. You can
then define your types with standard aggregate initialization syntax.

```cpp
struct my_struct
{
    int i;
    float f;
    
    my_struct(int i, float f)
        : i(i), f(f) { }
    
    bool valid() const
    {
        return i > 0 && f > 0.f;
    }
};

enum class my_enum
{
    value0,
    value1,
};

constexpr meta_type my_struct_type {
    .name = "my_struct",
    .bases = meta_tuple {},
    .fields = meta_tuple {
        meta_field {
            .name = "i",
            .field = &my_struct::i,
            .attributes = meta_tuple {},
        },
        meta_field {
            .name = "f",
            .field = &my_struct::f,
            .attributes = meta_tuple {},
        },
    },
    .functions = meta_tuple {
        meta_function {
            .name = "valid",
            .function = &my_struct::valid,
            .arguments = meta_tuple {},
            .attributes = meta_tuple {},
        },
    },
    .constructors = meta_tuple {
        meta_constructor {
            .constructor = +[](my_struct& value, const int i, const float f) {
                new (&value) my_struct{ i, f };
            },
            .arguments = meta_tuple {
                meta_argument {
                    .name = "i",
                    .type = meta_type_ref<const int> {},
                    .attributes = meta_tuple {},
                },
                meta_argument {
                    .name = "f",
                    .type = meta_type_ref<const float> {},
                    .attributes = meta_tuple {},
                },
            },
            .attributes = meta_tuple {},
        },
    },
    .attributes = meta_tuple {},
};

constexpr meta_enum my_enum_type {
    .name = "my_enum",
    .type = meta_type_ref<int>{},
    .values = meta_tuple {
        meta_enum_value {
            .name = "value0",
            .value = static_cast<int>(my_enum::value0),
            .attributes = meta_tuple {},
        },
        meta_enum_value {
            .name = "value1",
            .value = static_cast<int>(my_enum::value1),
            .attributes = meta_tuple {},
        },
    },
    .attributes = meta_tuple {},
};
```

To integrate with the querying interface, you need to provide an accessor function for your type and your enum.

```cpp
constexpr auto get_meta_type(const meta_adl<my_struct>)
{
    return my_struct_type;
}

constexpr auto get_meta_enum(const meta_adl<my_enum>)
{
    return my_enum_type;
}
```

And that's it! You should now be able to query your types at compile time.

```cpp
constexpr meta_type my_struct_type = meta::get_type<my_struct>();
constexpr meta_type my_enum_type = meta::get_type<my_enum>();
```

# Integrations

## CMake and Spore Codegen

Scripts in `cmake/SporeMeta.cmake` can be included to run [spore-codegen](https://github.com/sporacid/spore-codegen) on
your sources automatically. You will need to have libclang installed and available through your platform's `PATH`
variable. You can have a look at the `spore-codegen` repository for more information on how to install `libclang`.
Afterward, you should be able to add metadata generation to your CMake target.

```cmake
# With all defaults
spore_add_meta(my-target)

# With all arguments
spore_add_meta(
  my-target
  INPUT_DIRECTORY include
  INPUT_FILES **/*.hpp
  OUTPUT_DIRECTORY .codegen
  CODEGEN_TARGET my-target-meta
  IMPLICIT_TYPES
  IMPLICIT_ENUMS
  IMPLICIT_FIELDS
  IMPLICIT_FUNCTIONS
  IMPLICIT_CONSTRUCTORS
)
```

### Explicit Code Generation

By default, generation is disabled on all elements unless the `clang::annotate` attribute contains the corresponding
flag, e.g. `_spore_meta_type` for types, `_spore_meta_field` for fields, etc. You can use the macros defined in
`spore/meta/meta_macros.hpp` to enable generation. You can have a look at [this example](/examples/hello-world) for more
details on explicit generation.

```cpp
struct SPORE_META_TYPE() my_struct
{
    SPORE_META_FIELD()
    int i;
    
    SPORE_META_FIELD()
    float f;
    
    SPORE_META_CONSTRUCTOR()
    my_struct(int i, float f)
        : i(i), f(f) {}
    
    SPORE_META_FUNCTION()
    bool valid() const
    {
        return i > 0 && f > 0.f;
    }
};

enum class SPORE_META_ENUM() my_enum
{
   value0 SPORE_META_ENUM_VALUE(),
   value1 SPORE_META_ENUM_VALUE(),
};
```

### Implicit Code Generation

Alternatively, you can set the `IMPLICIT_` flags in the `spore_add_meta` function to generate metadata implicitly
without the need for attributes. You can have a look at [this example](/examples/implicit) for more
details on implicit generation.

```cmake
spore_add_meta(
  my-target
  IMPLICIT_TYPES
  IMPLICIT_FIELDS
)
```

```cpp
struct my_struct
{
    int i;
    float f;
    
    SPORE_META_FUNCTION()
    bool valid() const
    {
        return i > 0 && f > 0.f;
    }    
};
```

You can also use the `SPORE_META_IMPLICIT_TYPE` macro to explicitly define types that should be fully reflected.

```cpp
struct SPORE_META_IMPLICIT_TYPE() my_struct
{
    int i;
    float f;
    
    bool valid() const
    {
        return i > 0 && f > 0.f;
    }    
};
```

### Custom Code Generation

You could also create your own macros to mix and match which metadata are implicitly and explicitly defined.

```cpp
#define MY_META_STRUCT(...) SPORE_META_TYPE(_spore_meta_implicit_fields, __VA_ARGS__)
#define MY_META_CLASS(...)  SPORE_META_TYPE(_spore_meta_implicit_functions, _spore_meta_implicit_constructors, __VA_ARGS__)
```

## Vcpkg

`spore-meta` is available through the [spore-vcpkg repository](https://github.com/sporacid/spore-vcpkg).

1. In `vcpkg-configuration.json`:

   ```json
   {
     "registries": [
       {
         "kind": "git",
         "repository": "https://github.com/sporacid/spore-vcpkg",
         "baseline": "<baseline>",
         "packages": [
           "spore-meta"
         ]
       }
     ]
   }
   ```

2. In `vcpkg.json`:

   ```json
   {
     "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg.schema.json",
     "name": "project",
     "version": "1.0",
     "builtin-baseline": "<baseline>",
     "dependencies": [
       "spore-meta"
     ]
   }
   ```

# Query Interface

All querying interface functions are available through the `spore/meta/meta.hpp` header and the `spore::meta` namespace.

## For Each

`meta::for_each_*` family of function allows to iterate through a reflection structure's tuple, such as fields or
functions. The interface takes a single functor that should accept a single non-type template parameter of the given
struct type. Optionally, the functor can return `spore::meta_continue`, `spore::meta_break` or `spore::meta_return` to
control the flow of iteration. The end result of the control flow will be returned to the caller.

```cpp
constexpr auto func = []<meta_field field_v> {
   if constexpr (return_condition)
   {
      // stops iteration with int result
      constexpr int value = 0;
      return meta_return(value);
   }
   else if constexpr (break_condition)
   {
      // stops iteration
      return meta_break();
   }
   else
   {
      // continue iteration
      return meta_continue();
   }
};

constexpr any_meta_result auto result = meta::for_each_field<my_struct>(func);

if constexpr (is_meta_return_v<decltype(result)>)
{
   // iteration yielded result
   constexpr int value = result;
}
else if constexpr (is_meta_break_v<decltype(result)>)
{
   // iteration has stopped
}
else if constexpr (is_meta_continue_v<decltype(result)>)
{
   // iteration has continued
}
```

# Find

`meta::find_*` family of function allows to find a reflection structure within a tuple that matches a predicate. The
interface takes a single functor that should accept a single non-type template parameter of the given
struct type. The functor should return a boolean result to signify whether the structure matched or not. The first
matching structure will be returned. If no structure match the predicate, `meta_invalid` will be returned instead.

```cpp
constexpr auto predicate = []<meta_field field_v> {
   return field_v.name == "i";
};

constexpr auto field = meta::find_field<my_struct>(predicate);

if constexpr (meta::is_valid(field))
{
   // field has been found
}
else
{
   // field has not been found
}
```

If the predicate cannot be evaluated at compile-time, a `std::variant` of all structure types and `meta_invalid`,
will be returned instead.

```cpp
std::string name = "i";
const auto predicate = [&]<meta_field field_v> {
   return field_v.name == name;
};

const std::variant field = meta::find_field<my_struct>(predicate);

if (not std::holds_alternative<meta_invalid>(field))
{
   // field has been found
}
else
{
   // field has not been found
}
```

## Attributes

All attributes use the `spore-codegen` syntax within a `[[clang::annotate]]` attribute. You can find more information on
this syntax
on [this wiki page](https://github.com/sporacid/spore-codegen/blob/main/docs/ParserCpp.md#clang-annotate-attributes).
All attributes that start with `_spore_meta` are considered internal to this repository and won't be exposed through the
reflection structure.

## Extensions

User-defined extensions can be created for each type. The extensions must derive from `meta_extension` and they must be
literal types. You can override the function `make_extensions` to override the default extension factory.

```cpp
template <typename value_t>
struct extension : meta_extension
{
    using action_t = void(*)(value_t&);
    action_t action = nullptr;
};

template <typename value_t>
consteval extension make_extension()
{
    return extension<value_t> {
        .action = [](value_t& value) { action(value); };  
    };
}

template <typename value_t>
consteval any_meta_tuple_of<is_meta_extension> auto make_extensions(const meta_adl<value_t>, const meta_adl<void>)
{
    return meta_tuple {
        make_extension<value_t>(),
    };
}
```

You can use concepts to create extensions on types that are more constrained.

```cpp
template <typename value_t>
concept any_widget = /* ... */;

template <any_widget value_t>
consteval any_meta_tuple_of<is_meta_extension> auto make_extensions(const meta_adl<value_t>, const meta_adl<void>)
{
    return meta_tuple {
        make_extension<value_t>(),
        make_widget_extension<value_t>(),
    };
}
```

You can provide any number of overrides, but each override must be more constrained that previous ones to prevent
ambiguity.

# Examples

## Hello World

[Hello world example](examples/hello-world) shows multiple ways to print hello world using auto-generated reflection
metadata.

## Implicit Generation

[Implicit generation example](examples/implicit) shows how to generate reflection metadata implicitly without relying on
attributes and macros.

## JSON Serialization

[JSON serialization example](examples/json) shows how to generate JSON serialization automatically
with [nlohmann-json](https://github.com/nlohmann/json) and attributes.

## Templates

[Templates example](examples/templates) shows how to reflect type, function and constructor templates.

## Extensions

[Extensions example](examples/extensions) shows how to add custom extensions to your reflected types.

## External Project Integration

[External project integration example](https://github.com/sporacid/spore-meta-example) shows how to integrate with the
library from vcpkg and cmake with [spore-vcpkg](https://github.com/sporacid/spore-vcpkg) custom repository.