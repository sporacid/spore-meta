add_subdirectory(extensions)
add_subdirectory(hello-world)
add_subdirectory(implicit)
add_subdirectory(json)
add_subdirectory(templates)

if (SPORE_WITH_TYPE_REGISTRATION)
  add_subdirectory(registration)
endif ()