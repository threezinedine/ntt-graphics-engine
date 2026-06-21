macro(ntt_setup)
    set(NTT_OPTIONS)
    set(NTT_COMPILE_FLAGS)
    set(NTT_LINK_FLAGS)
    ntt_platform_detect()
    set(NTT_GRAPHICS_ENGINE_DIRECTORY "${CMAKE_SOURCE_DIR}")

    # build profile options
    option(CMAKE_BUILD_TYPE "Choose the type of build." "Debug")
    ntt_option_with_value(NTT_ENGINE ON)
    ntt_option_with_value(NTT_EDITOR OFF)
    ntt_option_with_value(NTT_TESTS OFF)

    ntt_option_with_value(NTT_GRAPHICS_DRIVER_GLFW ON)

    ntt_option_with_value(NTT_ENABLE_ASSERT ON)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "")
        set(NTT_DEBUG ON)
        list(APPEND NTT_OPTIONS "NTT_DEBUG")
        if (MSVC)
        else()
            list(APPEND NTT_COMPILE_FLAGS "-g" "-fno-omit-frame-pointer" "-O0")
            list(APPEND NTT_LINK_FLAGS "-rdynamic" "-no-pie")
        endif()
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(NTT_RELEASE ON)
        list(APPEND NTT_OPTIONS "NTT_RELEASE")
    elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(NTT_RELWITHDEBINFO ON)
        list(APPEND NTT_OPTIONS "NTT_RELWITHDEBINFO")
    else()
        message(WARNING "Unknown build type: ${CMAKE_BUILD_TYPE}, defaulting to Debug")
        set(NTT_DEBUG ON)
        list(APPEND NTT_OPTIONS "NTT_DEBUG")
    endif()

    set(NTT_APPLICATION_TYPES 
        NTT_EDITOR
        NTT_ENGINE
        NTT_TESTS
    )

    ntt_validate_radio_options(NTT_APPLICATION_TYPES)

    if (NTT_EDITOR)
        set(NTT_MAIN_FILE "src/main_editor.c")
        set(NTT_PROJECT_NAME "ntt-graphics-editor")
        set(NTT_INCLUDE_DIRS "inc")
        set(NTT_ADDITIONAL_SOURCES "")
        list(APPEND NTT_OPTIONS "NTT_GRAPHICS_ENGINE_DIRECTORY=${NTT_GRAPHICS_ENGINE_DIRECTORY}")
    elseif (NTT_ENGINE)
        set(NTT_MAIN_FILE "src/main_engine.c")
        set(NTT_PROJECT_NAME "ntt-graphics-engine")
        set(NTT_INCLUDE_DIRS "inc")
        set(NTT_ADDITIONAL_SOURCES "")
        list(APPEND NTT_OPTIONS "NTT_GRAPHICS_ENGINE_DIRECTORY=${NTT_GRAPHICS_ENGINE_DIRECTORY}")
    elseif (NTT_TESTS)
        set(NTT_MAIN_FILE "unittests/main_unittest.c")
        set(NTT_PROJECT_NAME "ntt-graphics-tests")
        set(NTT_INCLUDE_DIRS "unittests" "inc")
        file(
            GLOB_RECURSE 
            NTT_ADDITIONAL_SOURCES
            "unittests/*"
        )
        list(APPEND NTT_OPTIONS "NTT_GRAPHICS_ENGINE_DIRECTORY")
    else()
        message(FATAL_ERROR "Internal CMake error")
    endif()
endmacro()

function(ntt_validate_radio_options options)
    set(active_option "")
    foreach(option IN LISTS ${options})
        if (${${option}})
            if (active_option STREQUAL "")
                if (${${option}})
                    set(active_option ${option})
                endif()
            else()
                message(FATAL_ERROR "Multiple active options: \"${active_option}\" and \"${option}\"")
            endif()
        endif()
    endforeach()

    if (active_option STREQUAL "")
        message(FATAL_ERROR "No active option found in: ${options}")
    endif()
endfunction()

macro(ntt_platform_detect)
    if (EMSCRIPTEN)
        set(NTT_PLATFORM_WEB ON)
        list(APPEND NTT_OPTIONS "NTT_PLATFORM_WEB")
    elseif (WIN32)
        set(NTT_PLATFORM_WINDOWS ON)
        list(APPEND NTT_OPTIONS "NTT_PLATFORM_WINDOWS")
        
        if (MSVC)
            list(APPEND NTT_OPTIONS "NTT_MSVC")
            set(NTT_MSVC ON)
            set_property(GLOBAL PROPERTY USE_FOLDERS ON)
            list(APPEND NTT_COMPILE_FLAGS 
                    "/W4" 
                    "/permissive-"
                    "/WX")
        endif()
        
    elseif (UNIX)
        set(NTT_PLATFORM_UNIX ON)
        list(APPEND NTT_OPTIONS "NTT_PLATFORM_UNIX")
        list(APPEND NTT_COMPILE_FLAGS 
                "-Wall" 
                "-Wextra" 
                "-Wpedantic" 
                "-Werror")
    else()
        message(FATAL_ERROR "Unsupported platform")
    endif()
endmacro()

macro(ntt_option name)
    option(${name} "Enable ${name}" OFF)
    list(APPEND NTT_OPTIONS ${name})
endmacro()

macro(ntt_option_with_value name value)
    option(${name} "The ${name} with default value ${value}" ${value})
    list(APPEND NTT_OPTIONS ${name})
endmacro()

macro(ntt_log_options)
    message("NTT Configuration Options:")
    foreach(option IN LISTS NTT_OPTIONS)
        if (${${option}})
            message("   - ${option}=1")
        else()
            message("   - ${option}=0")
        endif()
    endforeach()
endmacro()

function(attach_options target)
    foreach(option IN LISTS NTT_OPTIONS)
        if (${${option}})
            target_compile_definitions(${target} PRIVATE ${option}=1)
        else()
            target_compile_definitions(${target} PRIVATE ${option}=0)
        endif()
    endforeach()
endfunction()

function(attach_flags target)
    target_compile_options(${target} PRIVATE ${NTT_COMPILE_FLAGS})
    target_link_libraries(${target} PRIVATE ${NTT_LINK_FLAGS})
endfunction()

function(ntt_find_package name folder)
    if (MSVC)
        set(CMAKE_FOLDER ${folder})
    endif()
    set(${name}_DIR "${CMAKE_SOURCE_DIR}/cmakes")
    find_package(${name} REQUIRED)
    if (MSVC)
        unset(CMAKE_FOLDER)
    endif()
endfunction()