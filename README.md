# What is Spore Meta

`spore-meta` is a C++23, header-only library to define compile-time reflection metadata for any given type. The library
is optionally integrated with [spore-codegen](https://github.com/sporacid/spore-codegen) to automatically generate this
metadata via `libclang`.

# Quick Start

If you're only interested in the reflection structure, you can copy the `include` directory in your project. You can
then define your types with standard aggregate initialization syntax.

```cpp
struct my_struct
{
    int i;
    float f;
    
    my_struct(int i_, float f_)
        : i(i_),
          f(f_)
    {
    }
    
    bool valid() const
    {
        return i > 0 && f > 0.f;
    }
};

constexpr meta_type my_struct_type {
    .name = "my_struct",
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
            .arguments = meta_tuple {
                meta_argument {
                    .name = "_this",
                    .type = meta_type_ref<my_struct> {},
                    .attributes = meta_tuple {},
                },
            },
            .attributes = meta_tuple {},
        },
    },
    .constructors = meta_tuple {
        meta_constructor {
            .constructor = [](my_struct& value, const int i_, const float f_) {
                new (&value) my_struct{ i_, f_ };
            },
            .arguments = meta_tuple {
                meta_argument {
                    .name = "const int",
                    .type = meta_type_ref<const int> {},
                    .attributes = meta_tuple {},
                },
                meta_argument {
                    .name = "const float",
                    .type = meta_type_ref<const float> {},
                    .attributes = meta_tuple {},
                },
            },
            .attributes = meta_tuple {},
        },
    },
    .attributes = meta_tuple {},
};

enum class my_enum
{
    value0,
    value1,
};

constexpr meta_enum my_enum_type {
    .name = "my_enum",
    .values = meta_tuple {
        meta_enum_value {
            .name = "value0",
            .value = static_cast<int>(my_enum::value0),
            .attributes = meta_tuple {},
        },
        meta_enum_value {
            .name = "value1",
            .value = static_cast<int>(my_enum::value1),
            .attributes = meta_tuple {
        },
    },
    .attributes = meta_tuple {},
};
```

To integrate with the querying interface, you would need to provide an accessor function for your types.

```cpp
template <typename func_t>
constexpr auto with_meta_type(meta_adl<my_struct>, func_t&& func)
{
    return func.template operator()<my_struct_type>();
}

template <typename func_t>
constexpr auto with_meta_enum(meta_adl<my_enum>, func_t&& func)
{
    return func.template operator()<my_enum_type>();
}
```

And that's it! You should now be able to query your types at compile time.

```cpp
constexpr auto my_struct_name = meta::get_name<my_struct>();
constexpr auto my_enum_name = meta::get_name<my_enum>();

meta::for_each_field<my_struct>([]<meta_field field_v> {});
meta::for_each_function<my_struct>([]<meta_function function_v> {});
meta::for_each_constructor<my_struct>([]<meta_constructor constructor_v> {});
meta::for_each_attribute<my_struct>([]<meta_attribute attribute_v> {});

meta::for_each_value<my_enum>([]<meta_enum_value value_v> {});
meta::for_each_attribute<my_enum>([]<meta_attribute attribute_v> {});

constexpr meta_field my_struct_field = meta::find_field<my_struct>([]<meta_field field_v> { return true; });
constexpr meta_function my_struct_function = meta::find_function<my_struct>([]<meta_function function_v> {});
constexpr meta_constructor my_struct_constuctor = meta::find_constructor<my_struct>([]<meta_constructor constructor_v> {});
constexpr meta_attribute my_struct_attribute = meta::find_attribute<my_struct>([]<meta_attribute attribute_v> {});

constexpr meta_enum_value my_enum_value = meta::for_each_value<my_enum>([]<meta_enum_value value_v> {});
constexpr meta_attribute my_enum_attribute = meta::for_each_attribute<my_enum>([]<meta_attribute attribute_v> {});
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
)
```

By default, generation is disabled on all elements unless the `clang::annotate` attribute contains the value
`_spore_meta_enabled`. You can use the macros defined in `spore/meta/meta_macros.hpp` to enable generation. You can have
a look at [examples](/examples) for more details.

```cpp
struct SPORE_META_TYPE() my_struct
{
    SPORE_META_FIELD()
    int i;
    
    SPORE_META_FIELD()
    float f;
    
    SPORE_META_CONSTRUCTOR()
    my_struct(int i_, float f_)
        : i(i_),
          f(f_)
    {
    }
    
    SPORE_META_FUNCTION()
    bool valid() const
    {
        return i > 0 && f > 0.f;
    }
};
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