# MIT License Copyright (c) 2024-2025 Tomáš Mark

# Function to automatically control debug information based on build type
function(apply_debug_info_control target)
    # Debug and RelWithDebInfo builds should always keep debug info
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        message(STATUS "${CMAKE_BUILD_TYPE} build - keeping debug info for ${target}")
        # For Debug builds, ensure NDEBUG is NOT defined to enable debug logging
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_definitions(${target} PRIVATE DEBUG)
        endif()
        return()
    endif()

    # Release and MinSizeRel builds automatically strip debug info
    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        message(STATUS "${CMAKE_BUILD_TYPE} build - removing debug info for ${target}")
    else()
        # For any other build type, keep debug info
        return()
    endif()

    # Get target type to determine strategy
    get_target_property(TARGET_TYPE ${target} TYPE)

    if(MSVC)
        # For MSVC, ensure no debug info
        target_compile_options(${target} PRIVATE /DNDEBUG)
        target_link_options(${target} PRIVATE /DEBUG:NONE)
    else()
        # For GCC/Clang, explicitly disable debug info generation
        target_compile_options(${target} PRIVATE -DNDEBUG -g0)

        # Strip executables after build (exclude Emscripten)
        if(TARGET_TYPE STREQUAL "EXECUTABLE" AND NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
            # First try to use CMAKE_STRIP if already set
            if(CMAKE_STRIP)
                set(STRIP_PROGRAM ${CMAKE_STRIP})
            # Then try environment variable STRIP
            elseif(DEFINED ENV{STRIP})
                set(STRIP_PROGRAM $ENV{STRIP})
            # Finally fall back to system strip
            else()
                find_program(STRIP_PROGRAM NAMES strip)
            endif()
            
            if(STRIP_PROGRAM)
                if(APPLE)
                    # macOS strip command (BSD-based)
                    add_custom_command(
                        TARGET ${target}
                        POST_BUILD
                        COMMAND ${STRIP_PROGRAM} -x $<TARGET_FILE:${target}>
                        COMMENT "Stripping symbols from ${target}")
                else()
                    # Linux strip command (GNU binutils)
                    add_custom_command(
                        TARGET ${target}
                        POST_BUILD
                        COMMAND ${STRIP_PROGRAM} --strip-all $<TARGET_FILE:${target}>
                        COMMENT "Stripping symbols from ${target}")
                endif()
            endif()
        endif()

        # For static libraries, ensure proper symbol table
        if(TARGET_TYPE STREQUAL "STATIC_LIBRARY")
            if(CMAKE_RANLIB)
                add_custom_command(
                    TARGET ${target}
                    POST_BUILD
                    COMMAND ${CMAKE_RANLIB} $<TARGET_FILE:${target}>
                    COMMENT "Creating symbol index for ${target}")
            endif()
        endif()
    endif()

    # Ensure NDEBUG is defined
    target_compile_definitions(${target} PRIVATE NDEBUG)
endfunction()
