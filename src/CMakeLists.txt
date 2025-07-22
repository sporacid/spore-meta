set(TARGET_NAME ${PROJECT_NAME}.lib)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/spore/meta/meta_version.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/spore/meta/meta_version.hpp
)

add_library(${TARGET_NAME} INTERFACE)

target_include_directories(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

include(GNUInstallDirs)

install(
  DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}"
  FILES_MATCHING PATTERN "**.hpp"
)

install(
  FILES "${PROJECT_SOURCE_DIR}/cmake/SporeMeta.cmake"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake"
)

install(
  DIRECTORY "${PROJECT_SOURCE_DIR}/codegen/"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/codegen"
)