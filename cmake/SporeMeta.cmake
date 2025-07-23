function(spore_add_meta SPORE_TARGET)
  cmake_parse_arguments(
    "SPORE_META"
    "IMPLICIT_TYPES;IMPLICIT_ENUMS;IMPLICIT_FIELDS;IMPLICIT_FUNCTIONS;IMPLICIT_CONSTRUCTORS"
    "INPUT_DIRECTORY;INPUT_FILES;OUTPUT_DIRECTORY;CODEGEN_TARGET"
    ""
    ${ARGN}
  )

  get_target_property(SPORE_TARGET_SOURCE_DIR ${SPORE_TARGET} SOURCE_DIR)
  get_target_property(SPORE_TARGET_BINARY_DIR ${SPORE_TARGET} BINARY_DIR)

  if (NOT SPORE_META_INPUT_DIRECTORY)
    set(SPORE_META_INPUT_DIRECTORY ${SPORE_TARGET_SOURCE_DIR}/include)
  endif ()

  if (NOT SPORE_META_INPUT_FILES)
    set(SPORE_META_INPUT_FILES **/*.hpp)
  endif ()

  if (NOT SPORE_META_OUTPUT_DIRECTORY)
    set(SPORE_META_OUTPUT_DIRECTORY ${SPORE_TARGET_BINARY_DIR}/.codegen/include)
  endif ()

  if (NOT SPORE_META_CODEGEN_TARGET)
    set(SPORE_META_CODEGEN_TARGET ${SPORE_TARGET}.codegen)
  endif ()

  # Might be set by toolchain (e.g. vcpkg)
  if (NOT SPORE_META_CODEGEN_DIRECTORY)
    set(SPORE_META_CODEGEN_DIRECTORY ${PROJECT_SOURCE_DIR}/codegen)
  endif ()

  if (NOT SPORE_META_IMPLICIT_TYPES OR NOT SPORE_META_IMPLICIT_ENUMS)
    string(
      CONCAT SPORE_META_CODEGEN_CONDITION
      "        condition:\n"
      "          type: any\n"
      "          value:"
    )

    if (NOT SPORE_META_IMPLICIT_TYPES)
      string(
        CONCAT SPORE_META_CODEGEN_CONDITION
        "${SPORE_META_CODEGEN_CONDITION}\n"
        "            - type: attribute\n"
        "              value:\n"
        "                _spore_meta_type: true"
      )
    endif ()

    if (NOT SPORE_META_IMPLICIT_ENUMS)
      string(
        CONCAT SPORE_META_CODEGEN_CONDITION
        "${SPORE_META_CODEGEN_CONDITION}\n"
        "            - type: attribute\n"
        "              value:\n"
        "                _spore_meta_enum: true"
      )
    endif ()
  endif ()

  configure_file(
    ${SPORE_META_CODEGEN_DIRECTORY}/codegen.yml.in
    ${SPORE_TARGET_BINARY_DIR}/.codegen/codegen.yml
  )

  include(SporeCodegen)

  spore_codegen(
    ${SPORE_TARGET}
    BIN_NAME spore::spore-codegen
    TARGET_NAME ${SPORE_META_CODEGEN_TARGET}
    TEMPLATES ${SPORE_META_CODEGEN_DIRECTORY}
    CONFIG ${SPORE_TARGET_BINARY_DIR}/.codegen/codegen.yml
    CACHE ${SPORE_TARGET_BINARY_DIR}/.codegen/cache.yml
    WORKING_DIRECTORY ${SPORE_TARGET_SOURCE_DIR}
    USER_DATA
      implicit_types=$<IF:$<BOOL:${SPORE_META_IMPLICIT_TYPES}>,true,false>
      implicit_enums=$<IF:$<BOOL:${SPORE_META_IMPLICIT_ENUMS}>,true,false>
      implicit_fields=$<IF:$<BOOL:${SPORE_META_IMPLICIT_FIELDS}>,true,false>
      implicit_functions=$<IF:$<BOOL:${SPORE_META_IMPLICIT_FUNCTIONS}>,true,false>
      implicit_constructors=$<IF:$<BOOL:${SPORE_META_IMPLICIT_CONSTRUCTORS}>,true,false>
  )

  get_target_property(SPORE_TARGET_TYPE ${SPORE_TARGET} TYPE)

  if (${SPORE_TARGET_TYPE} STREQUAL "INTERFACE_LIBRARY")
    set(SPORE_TARGET_INCLUDE_TYPE "INTERFACE")
  else ()
    set(SPORE_TARGET_INCLUDE_TYPE "PUBLIC")
  endif ()

  target_include_directories(
    ${SPORE_TARGET}
    ${SPORE_TARGET_INCLUDE_TYPE}
    ${SPORE_META_OUTPUT_DIRECTORY}
  )
endfunction()