cmake_minimum_required(VERSION 4.0)

project(NTTGraphicsEngine LANGUAGES C)

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