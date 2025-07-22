cmake_minimum_required(VERSION 3.20)

option(SPORE_BUILD_EXAMPLES "Whether to build examples" ON)

set(CMAKE_CXX_STANDARD 23)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/VERSION)
  file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION SPORE_VERSION)
else ()
  set(SPORE_VERSION 1.0.0)
endif ()

project(spore-meta VERSION ${SPORE_VERSION})

find_package(spore-codegen CONFIG REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${SPORE_CODEGEN_SCRIPTS})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(src)

if (SPORE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif ()