# MIT License Copyright (c) 2024-2025 Tomáš Mark

cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

# DotNameCpp Root project orchestrator

# DotNameCpp Version should be synchronized with git tags

project(
    DotNameCppTemplate
    VERSION 0.0.4
    LANGUAGES C CXX
    DESCRIPTION "DotNameC++ Template Project"
    HOMEPAGE_URL "https://github.com/tomasmark79/DotNameCpp")

# Build options - retrieve from python script
option(BUILD_LIBRARY "Build library" OFF)
option(BUILD_APPLICATION "Build application" OFF)

# Default: build both if nothing specified explicitly by user
if(NOT DEFINED BUILD_LIBRARY AND NOT DEFINED BUILD_APPLICATION)
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
    message(STATUS "Building DotNameBotLib")
endif()

if(BUILD_APPLICATION)
    include(cmake/project-application.cmake)
    message(STATUS "Building DotNameBot")
endif()
