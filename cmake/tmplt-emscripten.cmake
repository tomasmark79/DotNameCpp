# MIT License Copyright (c) 2024-2025 Tomáš Mark

# clear cache
# ./emcc --clear-cache

# SERVER SIDE DEPENDENCIES
# Apache directives example for CORS:
# <IfModule mod_headers.c>
#     Header set Cross-Origin-Opener-Policy "same-origin"
#     Header set Cross-Origin-Embedder-Policy "require-corp"
# </IfModule>

# Check if WebGL 2.0 is supported in the browser:
# const gl = canvas.getContext('webgl2');
# if (gl) {
#     console.log('WebGL 2.0 is active');
#     console.log('GL Version:', gl.getParameter(gl.VERSION));
# } else {
#     console.log('WebGL 2.0 not supported');
# }

function(emscripten target isHtml reqPthreads customPrePath)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
        return()
    endif()

    message(STATUS "Emscripten environment detected")

    # Define PLATFORM_WEB
    target_compile_definitions(${target} PRIVATE PLATFORM_WEB USE_WEBGL2)

    set_target_properties(${target} PROPERTIES OUTPUT_NAME "${target}")

    if(isHtml EQUAL 1)
        message(STATUS "HTML target requested")
        set_target_properties(${target} PROPERTIES SUFFIX ".html")
    endif()

    # Emscripten flags
    set(EMCC_FLAGS_WASM "-s WASM=1")
    set(EMCC_FLAGS_MP3 "-s USE_SDL_MIXER=2 -s SDL2_MIXER_FORMATS='[\"mp3\"]'")
    set(EMCC_FLAGS_OPTIMIZATION "-O3")
    set(EMCC_FLAGS_WEBGL2 "-s USE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2")
    set(EMCC_FLAGS_SDL2 "-s USE_SDL=2")
    set(EMCC_FLAGS_SDL2_IMAGE "-s USE_SDL_IMAGE=2")
    set(EMCC_FLAGS_SDL2_TTF "-s USE_SDL_TTF=2")
    set(EMCC_FLAGS_SDL2_MIXER "-s USE_SDL_MIXER=2")
    set(EMCC_FLAGS_ASYNCIFY "-s ASYNCIFY")
    set(EMCC_FLAGS_MEMORY "-s ALLOW_MEMORY_GROWTH=1")
    set(EMCC_FLAGS_EXCEPTIONS "-sNO_DISABLE_EXCEPTION_CATCHING")

    # Future flags (not used yet)
    # set(EMCC_FLAGS_GL_PROC "-s GL_ENABLE_GET_PROC_ADDRESS=1")
    # set(EMCC_FLAGS_ASSERTIONS "-s ASSERTIONS=1")

    # Pthread configuration
    if(reqPthreads EQUAL 1)
        set(EMCC_FLAGS_PTHREAD "-s USE_PTHREADS=1 -pthread")
        set(EMCC_FLAGS_PTHREAD_POOL "-s PTHREAD_POOL_SIZE=8")
    else()
        set(EMCC_FLAGS_PTHREAD "")
        set(EMCC_FLAGS_PTHREAD_POOL "")
    endif()

    # Default assets directory is share/${target}/assets
    if(NOT DEFINED customPrePath OR customPrePath STREQUAL "")
        set(customPrePath "--preload-file ${CMAKE_SOURCE_DIR}/assets@share/${target}/assets")
    endif()

    # Custom assets path - use CMAKE_SOURCE_DIR for modular structure
    if(target MATCHES "LibTester")
        set(customHtmlPath "--shell-file ${CMAKE_SOURCE_DIR}/assets/ems-mini.html")
    else()
        set(customHtmlPath "--shell-file ${CMAKE_SOURCE_DIR}/assets/ems-mini.html")
    endif()

    # Build compile flags
    set(COMPILE_FLAGS_LIST
        ${EMCC_FLAGS_OPTIMIZATION}
        ${EMCC_FLAGS_PTHREAD}
        ${EMCC_FLAGS_SDL2}
        ${EMCC_FLAGS_SDL2_IMAGE}
        ${EMCC_FLAGS_SDL2_TTF}
        ${EMCC_FLAGS_SDL2_MIXER}
        ${EMCC_FLAGS_EXCEPTIONS}
    )
    string(JOIN " " COMPILE_FLAGS_STRING ${COMPILE_FLAGS_LIST})

    # Build link flags
    set(LINK_FLAGS_LIST
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
        ${customHtmlPath}
    )
    string(JOIN " " LINK_FLAGS_STRING ${LINK_FLAGS_LIST})

    # Apply flags to target
    set_target_properties(${target} PROPERTIES
        COMPILE_FLAGS "${COMPILE_FLAGS_STRING}"
        LINK_FLAGS "${LINK_FLAGS_STRING}"
    )

    # Note: Assets are now accessed only through Emscripten virtual filesystem
    # No redundant copying needed - HTML loads assets via FS.readFile()

    # macOS specific frameworks (only required on macOS)
    if(APPLE)
        target_link_libraries(${target} PRIVATE
            "-framework IOKit"
            "-framework Cocoa"
            "-framework OpenGL"
        )
    endif()

endfunction()
