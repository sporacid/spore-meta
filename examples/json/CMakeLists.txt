include(SporeMeta)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp)

find_package(nlohmann_json CONFIG REQUIRED)

set(TARGET_NAME ${PROJECT_NAME}.examples.json)

add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_NAME}.lib nlohmann_json::nlohmann_json)

spore_add_meta(${TARGET_NAME})