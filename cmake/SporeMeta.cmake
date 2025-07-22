function(spore_add_meta SPORE_TARGET)
  cmake_parse_arguments(
    "SPORE_META"
    ""
    "INPUT_DIRECTORY;INPUT_FILES;OUTPUT_DIRECTORY;CODEGEN_TARGET"
    ""
    ${ARGN}
  )

  get_target_property(SPORE_TARGET_SOURCE_DIR ${SPORE_TARGET} SOURCE_DIR)
  get_target_property(SPORE_TARGET_BINARY_DIR ${SPORE_TARGET} BINARY_DIR)

  set(SPORE_META_CODEGEN_DIRECTORY ${PROJECT_SOURCE_DIR}/codegen)

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

  configure_file(
    ${PROJECT_SOURCE_DIR}/codegen/codegen.yml.in
    ${SPORE_TARGET_BINARY_DIR}/codegen.yml
  )

  include(SporeCodegen)

  spore_codegen(
    ${SPORE_TARGET}
      BIN_NAME spore::spore-codegen
      TARGET_NAME ${SPORE_META_CODEGEN_TARGET}
      CONFIG ${SPORE_TARGET_BINARY_DIR}/codegen.yml
      TEMPLATES ${SPORE_META_CODEGEN_DIRECTORY}
      CACHE ${SPORE_TARGET_BINARY_DIR}/.codegen/cache.yml
      WORKING_DIRECTORY ${SPORE_TARGET_SOURCE_DIR}
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