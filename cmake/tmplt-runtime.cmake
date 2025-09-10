# MIT License Copyright (c) 2024-2025 Tomáš Mark

# !!! -lstdc++ mus not be used in the final link command
# build/standalone/default/debug/CMakeFiles/index2.dir/link.txt

# ==============================================================================
# Static Runtime Configuration (SIMPLIFIED)
# ==============================================================================

# Define option with default value (safe to call multiple times)
if(NOT DEFINED USE_STATIC_RUNTIME)
    option(USE_STATIC_RUNTIME "Link against static runtime libraries" OFF)
endif()

# ==============================================================================
# Apply static runtime to target (does everything needed)
# ==============================================================================
function(apply_static_runtime TARGET_NAME)
    if(USE_STATIC_RUNTIME)
        message(STATUS "Applying static runtime to: ${TARGET_NAME}")

        if(MSVC)
            # MSVC runtime library property
            set_property(TARGET ${TARGET_NAME} PROPERTY MSVC_RUNTIME_LIBRARY
                                                        "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        else()
            if(WIN32)
                # MinGW - full static linking
                target_link_options(${TARGET_NAME} PRIVATE -static)
            elseif(APPLE)
                # MacOS - static C++ runtime only (clang doesn't support -static-libgcc)
                target_link_options(${TARGET_NAME} PRIVATE -static-libstdc++ -Wl,-dead_strip)
            else()
                # Linux/Unix - static C++ runtime only
                if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
                    # Emscripten - no special static linking flags needed Static linking is handled
                    # automatically by emscripten
                    message(STATUS "Emscripten detected - skipping static runtime flags")
                else()
                    # Native Linux - static C++ runtime only
                    target_link_options(${TARGET_NAME} PRIVATE -static-libgcc -static-libstdc++
                                        -Wl,--as-needed)
                endif()
            endif()
        endif()

        # For executables, prefer static libraries NOTE: If some dependencies are only available as
        # shared libraries (.so), comment out the following lines to avoid linking errors.
        # Alternatively, you can selectively disable static linking for specific targets. Example:
        # Force shared linking for zlib if only .so is available: find_package(ZLIB REQUIRED)
        # set_target_properties(ZLIB::ZLIB PROPERTIES IMPORTED_LINK_INTERFACE_LIBRARIES "")
        # target_link_libraries(${TARGET_NAME} PRIVATE ${ZLIB_LIBRARIES})

        # Absolute static linking - enabled by default
        get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)
        if(TARGET_TYPE STREQUAL "EXECUTABLE")
            set_target_properties(${TARGET_NAME} PROPERTIES LINK_SEARCH_START_STATIC ON
                                                            LINK_SEARCH_END_STATIC ON)
        endif()
    endif()
endfunction()
