# MIT License Copyright (c) 2024-2025 Tomáš Mark

cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

# CMake-Orchestrator

project(
    DotNameCpp
    # Version should be synchronized with git/github tags
    VERSION 0.0.5
    LANGUAGES C CXX
    DESCRIPTION "DotNameCpp is a C++ template project demonstrating best practices for modern C++ development with CMake."
    HOMEPAGE_URL "https://github.com/tomasmark79/DotNameCpp")

# Detect if this is the main project or included as a sub-project (via CPM)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(IS_MAIN_PROJECT ON)
else()
    set(IS_MAIN_PROJECT OFF)
endif()

# Build options - retrieve from python script or CPM consumer
option(BUILD_LIBRARY "Build library" ${IS_MAIN_PROJECT})
option(BUILD_APPLICATION "Build application" ${IS_MAIN_PROJECT})

# If used as a sub-project, build only the library
if(NOT IS_MAIN_PROJECT)
    set(BUILD_LIBRARY ON)
    set(BUILD_APPLICATION OFF)
    message(STATUS "DotNameCpp included as sub-project - building library only")
# Default: build both if nothing specified explicitly by user in main project
elseif(NOT DEFINED BUILD_LIBRARY AND NOT DEFINED BUILD_APPLICATION)
    set(BUILD_LIBRARY ON)
    set(BUILD_APPLICATION ON)
elseif(NOT BUILD_LIBRARY AND NOT BUILD_APPLICATION)
    message(STATUS "Both BUILD_LIBRARY and BUILD_APPLICATION are OFF - nothing will be built")
endif()

# Include common settings
include(cmake/project-common.cmake)

# Library is needed by application, so include if either is requested
if(BUILD_LIBRARY OR BUILD_APPLICATION)
    include(cmake/project-library.cmake)
    message(STATUS "Building library")
endif()

if(BUILD_APPLICATION)
    include(cmake/project-application.cmake)
    message(STATUS "Building application")
endif()
