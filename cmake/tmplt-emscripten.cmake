# MIT License Copyright (c) 2024-2025 Tomáš Mark

# clear cache ./emcc --clear-cache

set(USER_DEFINED_PTHREAD_POOL_SIZE "PTHREAD_POOL_SIZE=4")

function(emscripten target isHtml reqPthreads customPrePath)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
        return()
    endif()

    message(STATUS "Emscripten environment detected")

    # Define __EMSCRIPTEN__
    target_compile_definitions(${target} PRIVATE __EMSCRIPTEN__ USE_WEBGL2)

    set_target_properties(${target} PROPERTIES OUTPUT_NAME "${target}")

    if(isHtml EQUAL 1)
        message(STATUS "HTML target requested")
        set_target_properties(${target} PROPERTIES SUFFIX ".html")
    endif()

    # Emscripten flags
    set(EMCC_FLAGS_WASM "-s WASM=1")
    set(EMCC_FLAGS_MP3 "-s USE_SDL_MIXER=2 -s SDL2_MIXER_FORMATS='[\"mp3\"]'")
    set(EMCC_FLAGS_SDL2 "-s USE_SDL=2")
    set(EMCC_FLAGS_SDL2_IMAGE "-s USE_SDL_IMAGE=2")
    set(EMCC_FLAGS_SDL2_TTF "-s USE_SDL_TTF=2")
    set(EMCC_FLAGS_SDL2_MIXER "-s USE_SDL_MIXER=2")
    set(EMCC_FLAGS_ASYNCIFY "-s ASYNCIFY")
    set(EMCC_FLAGS_MEMORY "-s ALLOW_MEMORY_GROWTH=1")
    set(EMCC_FLAGS_EXCEPTIONS "-sNO_DISABLE_EXCEPTION_CATCHING")
    set(EMCC_FLAGS_WEBGL2 "-s USE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2")

    # Debug flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Debug build for Emscripten")
        set(EMCC_COMPILER_FLAGS_OPTIMIZATION "-O0")
        set(EMCC_LINKER_FLAGS_ASSERTIONS "-s ASSERTIONS=1")
        set(EMCC_LINKER_FLAGS_DEBUG "-gsource-map -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=1")
    else()
        set(EMCC_COMPILER_FLAGS_OPTIMIZATION "-O3")
        set(EMCC_LINKER_FLAGS_ASSERTIONS "")
        set(EMCC_LINKER_FLAGS_DEBUG "")
    endif()

    # Pthread configuration
    if(reqPthreads EQUAL 1)
        set(EMCC_FLAGS_PTHREAD "-s USE_PTHREADS=1 -pthread")
        set(EMCC_FLAGS_PTHREAD_POOL "-s ${USER_DEFINED_PTHREAD_POOL_SIZE}")
    else()
        set(EMCC_FLAGS_PTHREAD "")
        set(EMCC_FLAGS_PTHREAD_POOL "")
    endif()

    # Default assets directory is share/${target}/assets
    if(NOT DEFINED customPrePath OR customPrePath STREQUAL "")
        set(customPrePath "--preload-file ${CMAKE_SOURCE_DIR}/assets@share/${target}/assets")
    endif()

    # Custom HTML shell file
    if(target MATCHES "LibTester")
        set(customHtmlPath "--shell-file ${CMAKE_SOURCE_DIR}/assets/ems-mini.html")
    else()
        set(customHtmlPath "--shell-file ${CMAKE_SOURCE_DIR}/assets/ems-mini.html")
    endif()

    # Join compiler flags into strings
    set(COMPILE_FLAGS_LIST
        ${EMCC_COMPILER_FLAGS_OPTIMIZATION}
        ${EMCC_FLAGS_PTHREAD}
        ${EMCC_FLAGS_SDL2}
        ${EMCC_FLAGS_SDL2_IMAGE}
        ${EMCC_FLAGS_SDL2_TTF}
        ${EMCC_FLAGS_SDL2_MIXER}
        ${EMCC_FLAGS_EXCEPTIONS})
    string(JOIN " " COMPILE_FLAGS_STRING ${COMPILE_FLAGS_LIST})

    # Join linker flags into strings
    set(LINK_FLAGS_LIST
        ${EMCC_LINKER_FLAGS_DEBUG}
        ${EMCC_LINKER_FLAGS_ASSERTIONS}
        ${EMCC_FLAGS_GSOURCE_MAP}
        ${EMCC_FLAGS_WEBGL2}
        ${EMCC_FLAGS_MP3}
        ${EMCC_FLAGS_ASYNCIFY}
        ${EMCC_FLAGS_MEMORY}
        ${EMCC_FLAGS_WASM}
        ${EMCC_FLAGS_PTHREAD}
        ${EMCC_FLAGS_PTHREAD_POOL}
        ${EMCC_FLAGS_SDL2}
        ${EMCC_FLAGS_SDL2_IMAGE}
        ${EMCC_FLAGS_SDL2_TTF}
        ${EMCC_FLAGS_SDL2_MIXER}
        ${EMCC_FLAGS_EXCEPTIONS}
        ${customPrePath}
        ${customHtmlPath})
    string(JOIN " " LINK_FLAGS_STRING ${LINK_FLAGS_LIST})

    # Apply flags to target
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS_STRING}"
                                               LINK_FLAGS "${LINK_FLAGS_STRING}")

    # Note: Assets are now accessed only through Emscripten virtual filesystem No redundant copying
    # needed - HTML loads assets via FS.readFile()

    # macOS specific frameworks (only required on macOS)
    if(APPLE)
        target_link_libraries(${target} PRIVATE "-framework IOKit" "-framework Cocoa"
                                                "-framework OpenGL")
    endif()

endfunction()
