# MIT License Copyright (c) 2024-2026 Tomáš Mark

include(CheckCXXCompilerFlag)

function(apply_hardening target)
    if(NOT ENABLE_HARDENING)
        return()
    endif()

    message(STATUS "Enabling hardening for target ${target}")
    
    if(MSVC)
        set(HARDENING_COMPILE_FLAGS
            /GS        # Buffer security check
            /DYNAMICBASE # Random base address
            /NXCOMPAT  # Data execution prevention
            /CETCOMPAT # Control flow protection
        )
        
        set(HARDENING_LINK_FLAGS
            /HIGHENTROPYVA # High entropy ASLR
            /DYNAMICBASE   # ASLR
            /NXCOMPAT      # DEP
            /GUARD:CF      # Control Flow Guard
        )

        target_compile_options(${target} PRIVATE ${HARDENING_COMPILE_FLAGS})
        target_link_options(${target} PRIVATE ${HARDENING_LINK_FLAGS})
    else()
        set(HARDENING_FLAGS
            -fstack-protector-strong # Stack smashing protection
            -fPIE # Position independent executable
            -D_FORTIFY_SOURCE=2 # Additional buffer overflow checks
        )

        # Add platform-specific flags
        if(NOT APPLE)
            list(APPEND HARDENING_FLAGS
                -fstack-clash-protection # Stack clash protection
                -fcf-protection # Control flow protection
                -fno-common # Disallow common symbols
            )
        endif()

        # Add Clang-specific flags, but not on Apple platforms
        if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT APPLE)
            list(APPEND HARDENING_FLAGS
                -fsanitize=safe-stack # Safe stack protection
                -fsanitize=cfi # Control flow integrity
            )
        endif()

        target_compile_options(${target} PRIVATE ${HARDENING_FLAGS})
        
        if(NOT APPLE AND NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
            target_link_options(${target} PRIVATE 
                -Wl,-z,relro # Relocation read-only
                -Wl,-z,now # No lazy binding
                -Wl,-z,noexecstack # No executable stack
                -Wl,-z,separate-code # Separate code and data
            )
        endif()
    endif()
endfunction()