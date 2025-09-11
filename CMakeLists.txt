# MIT License Copyright (c) 2024-2025 Tomáš Mark

cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

# DotNameCpp Root project orchestrator

# DotNameCpp Version should be synchronized with git tags

project(
    DotNameCpp
    VERSION 0.0.2
    LANGUAGES C CXX ASM
    DESCRIPTION "DotNameC++ Template Project"
    HOMEPAGE_URL "https://github.com/tomasmark79/DotNameCpp")

# Build options - retrieve from python script
option(BUILD_LIBRARY "Build library" OFF)
option(BUILD_STANDALONE "Build standalone" OFF)

# Default: build both if nothing specified explicitly by user
if(NOT DEFINED BUILD_LIBRARY AND NOT DEFINED BUILD_STANDALONE)
    set(BUILD_LIBRARY ON)
    set(BUILD_STANDALONE ON)
elseif(NOT BUILD_LIBRARY AND NOT BUILD_STANDALONE)
    message(STATUS "Both BUILD_LIBRARY and BUILD_STANDALONE are OFF - nothing will be built")
endif()

# Include common settings
include(cmake/project-common.cmake)

# Library is needed by standalone, so include if either is requested
if(BUILD_LIBRARY OR BUILD_STANDALONE)
    include(cmake/project-library.cmake)
    message(STATUS "Building DotNameLib")
endif()

if(BUILD_STANDALONE)
    include(cmake/project-standalone.cmake)
    message(STATUS "Building DotNameApp")
endif()
