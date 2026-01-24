# MIT License Copyright (c) 2024-2026 Tomáš Mark

cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

# ==============================================================================
# TEST-SPECIFIC CONFIGURATION
# ==============================================================================
include(${CMAKE_CURRENT_LIST_DIR}/project-common.cmake)

project(${TEST_NAME} LANGUAGES CXX)
include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)
include(GoogleTest)

# ==============================================================================
# Set target properties
# ==============================================================================

# A. Way given by CPM.cmake
CPMAddPackage(
    NAME GTest
    GITHUB_REPOSITORY google/googletest
    VERSION 1.17.0
    OPTIONS "INSTALL_GTEST OFF")
# Ensure GTest targets are available
if(GTest_ADDED)
    # Make sure include directories are properly set
    target_include_directories(gtest PUBLIC $<BUILD_INTERFACE:${gtest_SOURCE_DIR}/include>
                                            $<BUILD_INTERFACE:${gtest_SOURCE_DIR}>)
    target_include_directories(gtest_main PUBLIC $<BUILD_INTERFACE:${gtest_SOURCE_DIR}/include>
                                                 $<BUILD_INTERFACE:${gtest_SOURCE_DIR}>)
    # Apply sanitizers to GTest targets to ensure compatibility with tested code
    apply_sanitizers(gtest)
    apply_sanitizers(gtest_main)
endif()
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# B. Way given via (Conan) find_package(GTest REQUIRED)

# configure the test executable
add_executable(${TEST_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_NAME} PRIVATE GTest::gtest GTest::gtest_main
                                           dotname::${TEST_NAME_LOWER}_application_common)
set_target_properties(${TEST_NAME} PROPERTIES OUTPUT_NAME "${TEST_NAME}")

# Skip test discovery when cross-compiling
if(DOTNAME_CROSSCOMPILING)
    message(STATUS "Skipping gtest_discover_tests for ${TEST_NAME} (cross-compiling)")
else()
    gtest_discover_tests(${TEST_NAME})
    # Alternative: explicit test registration
    add_test(NAME ${TEST_NAME}_run COMMAND ${TEST_NAME})
endif()

# ==============================================================================
# Apply common target settings (includes sanitizers, debug info, etc.)
# ==============================================================================
apply_common_target_settings(${TEST_NAME})

# emscripten handler
include(${CMAKE_CURRENT_LIST_DIR}/tmplt-emscripten.cmake)
if(NOT DEFINED target)
    # Ensure the emscripten() helper has a default 'target' variable to reference
    set(target ${TEST_NAME})
endif()
emscripten(${TEST_NAME} 1 0 "--preload-file ${CMAKE_SOURCE_DIR}/assets@share/${target}/assets")
