# MIT License Copyright (c) 2024-2026 Tomáš Mark

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
include(CheckIPOSupported) # require CMake 3.9
function(apply_ipo target)
    if(NOT ENABLE_IPO)
        return()
    endif()

    # Check if IPO/LTO is supported
    check_ipo_supported(RESULT ipo_supported OUTPUT error_message)
    
    if(ipo_supported)
        message(STATUS "Enabling IPO/LTO for target ${target}")
        set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "IPO/LTO is not supported: ${error_message}")
    endif()
endfunction()