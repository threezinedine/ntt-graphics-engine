if (TARGET ntt-glfw)
    return()
endif()

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/externals/glfw"
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/threezinedine/glad.git
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/externals/glad"
)

FetchContent_MakeAvailable(glad)

add_library(ntt-glfw INTERFACE)
target_link_libraries(ntt-glfw INTERFACE glfw glad)