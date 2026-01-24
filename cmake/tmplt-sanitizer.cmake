# MIT License Copyright (c) 2024-2026 Tomáš Mark

# ==============================================================================
# Sanitizer mode decoder
# ==============================================================================
# SANITIZER_MODE values:
#   0 = None           - No sanitizers
#   1 = ASAN           - Address Sanitizer
#   2 = UBSAN          - Undefined Behavior Sanitizer
#   3 = ASAN + UBSAN   - Address + Undefined
#   4 = TSAN           - Thread Sanitizer
#   5 = TSAN + UBSAN   - Thread + Undefined
#   6 = MSAN           - Memory Sanitizer (Clang only)
#   7 = MSAN + UBSAN   - Memory + Undefined (Clang only)

function(decode_sanitizer_mode OUT_ASAN OUT_UBSAN OUT_TSAN OUT_MSAN)
    set(_asan OFF)
    set(_ubsan OFF)
    set(_tsan OFF)
    set(_msan OFF)

    if(SANITIZER_MODE EQUAL 1)
        set(_asan ON)
    elseif(SANITIZER_MODE EQUAL 2)
        set(_ubsan ON)
    elseif(SANITIZER_MODE EQUAL 3)
        set(_asan ON)
        set(_ubsan ON)
    elseif(SANITIZER_MODE EQUAL 4)
        set(_tsan ON)
    elseif(SANITIZER_MODE EQUAL 5)
        set(_tsan ON)
        set(_ubsan ON)
    elseif(SANITIZER_MODE EQUAL 6)
        set(_msan ON)
    elseif(SANITIZER_MODE EQUAL 7)
        set(_msan ON)
        set(_ubsan ON)
    elseif(NOT SANITIZER_MODE EQUAL 0)
        message(WARNING "Invalid SANITIZER_MODE=${SANITIZER_MODE}, using 0 (none)")
    endif()

    set(${OUT_ASAN}
        ${_asan}
        PARENT_SCOPE)
    set(${OUT_UBSAN}
        ${_ubsan}
        PARENT_SCOPE)
    set(${OUT_TSAN}
        ${_tsan}
        PARENT_SCOPE)
    set(${OUT_MSAN}
        ${_msan}
        PARENT_SCOPE)
endfunction()

function(get_sanitizer_mode_description OUT_DESC)
    set(_desc "None")
    if(SANITIZER_MODE EQUAL 1)
        set(_desc "ASAN")
    elseif(SANITIZER_MODE EQUAL 2)
        set(_desc "UBSAN")
    elseif(SANITIZER_MODE EQUAL 3)
        set(_desc "ASAN+UBSAN")
    elseif(SANITIZER_MODE EQUAL 4)
        set(_desc "TSAN")
    elseif(SANITIZER_MODE EQUAL 5)
        set(_desc "TSAN+UBSAN")
    elseif(SANITIZER_MODE EQUAL 6)
        set(_desc "MSAN")
    elseif(SANITIZER_MODE EQUAL 7)
        set(_desc "MSAN+UBSAN")
    endif()
    set(${OUT_DESC}
        ${_desc}
        PARENT_SCOPE)
endfunction()

# ==============================================================================
# Individual sanitizer application functions
# ==============================================================================
function(apply_address_sanitizer TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    if(NOT APPLE)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=address -static-libasan)
    else()
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=address)
    endif()
    set(ENV{ASAN_OPTIONS} "detect_leaks=1:strict_string_checks=1:verbosity=1:log_threads=1")
endfunction()

function(apply_undefined_sanitizer TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=undefined)
    if(NOT APPLE)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=undefined -static-libubsan)
    else()
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=undefined)
    endif()
endfunction()

function(apply_thread_sanitizer TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=thread)
    if(NOT APPLE)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=thread -static-libtsan)
    else()
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=thread -static-libtsan)
    endif()
endfunction()

function(apply_memory_sanitizer TARGET_NAME)
    # MSan requires Clang/LLVM and is not generally supported on Apple platforms
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(
            FATAL_ERROR "MemorySanitizer requires Clang/LLVM (set CMAKE_CXX_COMPILER to clang++)")
    endif()
    if(APPLE)
        message(FATAL_ERROR "MemorySanitizer is not supported on Apple platforms in this project")
    endif()
    target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=memory -fno-omit-frame-pointer)
    target_link_options(${TARGET_NAME} PRIVATE -fsanitize=memory)
    set(ENV{MSAN_OPTIONS} "verbosity=1:log_threads=1")
endfunction()

function(apply_sanitizers TARGET_NAME)
    # Decode the SANITIZER_MODE into individual flags
    decode_sanitizer_mode(_use_asan _use_ubsan _use_tsan _use_msan)
    get_sanitizer_mode_description(_san_desc)

    if(SANITIZER_MODE GREATER 0)
        message(STATUS "Sanitizer[${_san_desc}] -> ${TARGET_NAME}")
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

        if(_use_asan)
            apply_address_sanitizer(${TARGET_NAME})
        endif()

        if(_use_ubsan)
            apply_undefined_sanitizer(${TARGET_NAME})
        endif()

        if(_use_tsan)
            apply_thread_sanitizer(${TARGET_NAME})
        endif()

        if(_use_msan)
            apply_memory_sanitizer(${TARGET_NAME})
        endif()

    elseif(SANITIZER_MODE GREATER 0)
        message(WARNING "Sanitizers are only supported for GCC and Clang")
    endif()
endfunction()
