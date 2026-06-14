cmake_minimum_required(VERSION 4.0)

project(NTTGraphicsEngine LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmakes")
include(ntt-configure)
ntt_setup()

ntt_option_with_value(NTT_STRICT_MEMORY_MANAGEMENT OFF)

file(
    GLOB_RECURSE 
    NTT_GRAPHICS_ENGINE_SOURCES
    "src/**/*.c"
)

file(
    GLOB_RECURSE 
    NTT_GRAPHICS_ENGINE_HEADERS
    "inc/engine/*.h"
)

add_executable(
    ${NTT_PROJECT_NAME}
    ${NTT_GRAPHICS_ENGINE_SOURCES}
    ${NTT_GRAPHICS_ENGINE_HEADERS}
    ${NTT_MAIN_FILE}
    ${NTT_ADDITIONAL_SOURCES}
)

target_include_directories(
    ${NTT_PROJECT_NAME}
    PRIVATE
    ${NTT_INCLUDE_DIRS}
)

attach_options(${NTT_PROJECT_NAME})
attach_flags(${NTT_PROJECT_NAME})

ntt_find_package(ntt-glfw "Dependencies")

target_link_libraries(
    ${NTT_PROJECT_NAME}
    PRIVATE
    ntt-glfw
)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${NTT_PROJECT_NAME})
endif()

ntt_log_options()