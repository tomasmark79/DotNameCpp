# MIT License Copyright (c) 2024-2026 Tomáš Mark

# Useful Emscripten commands: clear cache ./emcc --clear-cache

# Chrome extension for better debugging - C/C++ DevTools Support (DWARF)
# https://chromewebstore.google.com/detail/cc++-devtools-support-dwa/pdcpmagijalfljmkmjngeonclgbbannb

function(emscripten target isHtml reqPthreads customPrePath)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
        return()
    endif()

    message(STATUS "Emscripten environment detected")

    # Define __EMSCRIPTEN__
    target_compile_definitions(${target} PRIVATE __EMSCRIPTEN__ USE_WEBGL2)

    # Set output name
    set_target_properties(${target} PROPERTIES OUTPUT_NAME "${target}")

    # Set output directory
    if(isHtml EQUAL 1)
        message(STATUS "HTML target requested")
        set_target_properties(${target} PROPERTIES SUFFIX ".html")
    endif()

    # Debug / Release configuration
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Debug build for Emscripten")
        set(FLAG_OPTIMIZATION "-O0")
        set(FLAG_GSOURCE_MAP "-gsource-map")
        # Safe heap and stack overflow check flags for debug builds
        set(FLAG_SAFE_HEAP "-s SAFE_HEAP=1")
        set(FLAG_STACK_OVERFLOW_CHECK "-s STACK_OVERFLOW_CHECK=1")
        set(FLAG_COMPILER_DEBUG "-g3")
        set(FLAG_LINKER_DEBUG "${FLAG_SAFE_HEAP} ${FLAG_STACK_OVERFLOW_CHECK}")
    else()
        set(FLAG_OPTIMIZATION "-O3")
        set(FLAG_GSOURCE_MAP "")
        set(FLAG_COMPILER_DEBUG "")
        set(FLAG_LINKER_DEBUG "")
        set(FLAG_SAFE_HEAP "")
        set(FLAG_STACK_OVERFLOW_CHECK "")
    endif()

    # WebAssembly output
    set(FLAG_WASM_1 "-s WASM=1")

    # WebGL2 support (compiler and linker)
    set(FLAG_WEBGL2 "-s USE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2")

    # C++ Exceptions Support (compiler)
    set(FLAG_CPP_EXCEPTIONS "-sNO_DISABLE_EXCEPTION_CATCHING")

    # SDL2 and related libraries
    set(FLAG_SDL2 "-s USE_SDL=2")
    set(FLAG_SDL2_IMAGE "-s USE_SDL_IMAGE=2")
    set(FLAG_SDL2_TTF "-s USE_SDL_TTF=2")
    set(FLAG_SDL2_MIXER_WITH_MP3 "-s USE_SDL_MIXER=2 -s SDL2_MIXER_FORMATS='[\"mp3\"]'")

    # Whether to support async operations in the compiled code. This makes it possible to call JS
    # functions from synchronous-looking code in C/C++. 1 (default): Run binaryen’s Asyncify pass to
    # transform the code using asyncify. This emits a normal wasm file in the end, so it works
    # everywhere, but it has a significant cost in terms of code size and speed. See
    # https://emscripten.org/docs/porting/asyncify.html 2 (deprecated): Use -sJSPI instead.
    set(FLAG_ASYNCIFY "-s ASYNCIFY=1")

    # If false, we abort with an error if we try to allocate more memory than we can
    # (INITIAL_MEMORY). If true, we will grow the memory arrays at runtime, seamlessly and
    # dynamically
    set(FLAG_MEMORY "-s ALLOW_MEMORY_GROWTH=1")

    # Whether we should add runtime assertions. This affects both JS and how system libraries are
    # built. ASSERTIONS == 2 gives even more runtime checks, that may be very slow. That includes
    # internal dlmalloc assertions, for example. ASSERTIONS defaults to 0 in optimized builds (-O1
    # and above).
    set(FLAG_ASSERTIONS "-s ASSERTIONS=1")

    # Pthread configuration
    if(reqPthreads EQUAL 1)
        set(FLAG_PTHREAD "-s USE_PTHREADS=1 -pthread")
        set(FLAG_PTHREAD_POOL "-s PTHREAD_POOL_SIZE=4")
    else()
        set(FLAG_PTHREAD "")
        set(FLAG_PTHREAD_POOL "")
    endif()

    # Default assets directory is share/${target}/assets
    if(NOT DEFINED customPrePath OR customPrePath STREQUAL "")
        set(customPrePath "--preload-file ${CMAKE_SOURCE_DIR}/assets@share/${target}/assets")
    endif()

    # Custom HTML shell file
    set(customHtmlPath "--shell-file ${CMAKE_SOURCE_DIR}/assets/ems-mini.html")

    # Join compiler flags into strings
    set(COMPILE_FLAGS_LIST
        ${FLAG_COMPILER_DEBUG}
        ${FLAG_OPTIMIZATION}
        ${FLAG_PTHREAD}
        ${FLAG_SDL2}
        ${FLAG_SDL2_IMAGE}
        ${FLAG_SDL2_TTF}
        ${FLAG_SDL2_MIXER_WITH_MP3}
        ${FLAG_CPP_EXCEPTIONS})
    string(JOIN " " COMPILE_FLAGS_STRING ${COMPILE_FLAGS_LIST})

    # Join linker flags into strings
    set(LINK_FLAGS_LIST
        ${FLAG_LINKER_DEBUG}
        ${FLAG_WEBGL2}
        ${FLAG_WASM_1}
        ${FLAG_GSOURCE_MAP}
        ${FLAG_ASYNCIFY}
        ${FLAG_MEMORY}
        ${FLAG_PTHREAD}
        ${FLAG_PTHREAD_POOL}
        ${FLAG_SDL2}
        ${FLAG_SDL2_IMAGE}
        ${FLAG_SDL2_TTF}
        ${FLAG_SDL2_MIXER_WITH_MP3}
        ${FLAG_CPP_EXCEPTIONS}
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
