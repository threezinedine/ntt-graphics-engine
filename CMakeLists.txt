cmake_minimum_required(VERSION 4.0)

project(NTTGraphicsEngine LANGUAGES C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmakes")
include(ntt-configure)
ntt_setup()

file(
    GLOB_RECURSE 
    NTT_GRAPHICS_ENGINE_SOURCES
    "src/*.c"
)

file(
    GLOB_RECURSE 
    NTT_GRAPHICS_ENGINE_HEADERS
    "inc/engine/*.h"
)

add_executable(
    ${PROJECT_NAME}
    ${NTT_GRAPHICS_ENGINE_SOURCES}
    ${NTT_GRAPHICS_ENGINE_HEADERS}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    "inc"
)

attach_options(${PROJECT_NAME})
attach_flags(${PROJECT_NAME})

ntt_log_options()