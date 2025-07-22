set(TARGET_NAME ${PROJECT_NAME}.lib)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/spore/meta/meta_version.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/spore/meta/meta_version.hpp
)

add_library(${TARGET_NAME} INTERFACE)

target_include_directories(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../include)