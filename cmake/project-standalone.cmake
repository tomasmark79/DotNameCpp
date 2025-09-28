# MIT License Copyright (c) 2024-2025 Tomáš Mark

# ==============================================================================
# STANDALONE-SPECIFIC CONFIGURATION
# ==============================================================================
include(${CMAKE_CURRENT_LIST_DIR}/project-common.cmake)

project(
    ${STANDALONE_NAME}
    LANGUAGES C CXX ASM
    DESCRIPTION "template Copyright (c) 2024 TomasMark [at] digitalspace.name"
    HOMEPAGE_URL "https://github.com/tomasmark79/DotNameCpp")

# ==============================================================================
# Build guards
# ==============================================================================
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
        WARNING
            "In-source builds. Please make a new directory (called a Build directory) and run CMake from there."
    )
endif()

# ==============================================================================
# Standalone dependencies
# ==============================================================================
# Note: DotNameLib target should be available from orchestrator
CPMAddPackage("gh:cpm-cmake/CPMLicenses.cmake@0.0.7")
cpm_licenses_create_disclaimer_target(
    write-licenses-${STANDALONE_NAME}
    "${CMAKE_CURRENT_BINARY_DIR}/${STANDALONE_NAME}_third_party.txt" "${CPM_PACKAGES}")
CPMAddPackage(
    GITHUB_REPOSITORY jarro2783/cxxopts
    VERSION 3.2.1
    OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL NO")

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    CPMAddPackage(
        NAME cpptrace
        GITHUB_REPOSITORY jeremy-rifkin/cpptrace
        VERSION 1.0.4)
endif()

# ==============================================================================
# Standalone source files
# ==============================================================================
gather_sources(sources ${CMAKE_CURRENT_SOURCE_DIR}/standalone/src)
list(APPEND sources ${CMAKE_CURRENT_SOURCE_DIR}/standalone/src/Main.cpp)

# ==============================================================================
# Create standalone target
# ==============================================================================
add_executable(${STANDALONE_NAME})
target_sources(${STANDALONE_NAME} PRIVATE ${sources})

# Apply common target settings
apply_common_target_settings(${STANDALONE_NAME})

# Link with library
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    target_link_libraries(${STANDALONE_NAME} PRIVATE DotNameLib cxxopts::cxxopts cpptrace::cpptrace)
else()
    target_link_libraries(${STANDALONE_NAME} PRIVATE DotNameLib cxxopts::cxxopts)
endif()

# Link TBB only on Linux X86 where it's properly supported
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    target_link_libraries(${STANDALONE_NAME} PRIVATE tbb)
    message(STATUS "TBB enabled for Linux X86 build")
else()
    message(STATUS "TBB disabled for ${CMAKE_SYSTEM_NAME} - using fallback implementation")
endif()

# ==============================================================================
# Asset processing and Emscripten configuration
# ==============================================================================
include(${CMAKE_CURRENT_LIST_DIR}/tmplt-assets.cmake)
apply_assets_processing_standalone()

include(${CMAKE_CURRENT_LIST_DIR}/tmplt-emscripten.cmake)
emscripten(${STANDALONE_NAME} 1 1 "")

# ==============================================================================
# Tests configuration
# ==============================================================================
if(ENABLE_GTESTS)
    message(STATUS "GTESTS enabled")
    include(CTest) # Enable testing at the root level
    add_library(${TEST_NAME_LOWER}_standalone_common INTERFACE)
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
        target_link_libraries(${TEST_NAME_LOWER}_standalone_common
                              INTERFACE ${LIBRARY_NAME} cxxopts::cxxopts cpptrace::cpptrace)
    else()
        target_link_libraries(${TEST_NAME_LOWER}_standalone_common INTERFACE ${LIBRARY_NAME}
                                                                             cxxopts::cxxopts)
    endif()

    # Link TBB only on Linux for tests
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        target_link_libraries(${TEST_NAME_LOWER}_standalone_common INTERFACE tbb)
    endif()

    add_library(dotname::${TEST_NAME_LOWER}_standalone_common ALIAS
                ${TEST_NAME_LOWER}_standalone_common)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/standalone/tests tests)
endif()

# ==============================================================================
# Installation
# ==============================================================================
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR})

# Standard installation for native platforms
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    install(TARGETS ${STANDALONE_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
    # Special installation for Emscripten - install all generated files
    install(TARGETS ${STANDALONE_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    # Install additional Emscripten files (js, wasm, data) These files are generated alongside the
    # .html file
    get_target_property(TARGET_SUFFIX ${STANDALONE_NAME} SUFFIX)
    if(TARGET_SUFFIX STREQUAL ".html")
        set(BASE_NAME "${STANDALONE_NAME}")

        # Install .js file (JavaScript wrapper)
        install(
            FILES "${CMAKE_CURRENT_BINARY_DIR}/bin/${BASE_NAME}.js"
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            OPTIONAL)

        # Install .wasm file (WebAssembly binary)
        install(
            FILES "${CMAKE_CURRENT_BINARY_DIR}/bin/${BASE_NAME}.wasm"
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            OPTIONAL)

        # Install .data file (preloaded assets)
        install(
            FILES "${CMAKE_CURRENT_BINARY_DIR}/bin/${BASE_NAME}.data"
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            OPTIONAL)

        # Install .wasm.map file (WebAssembly source map for debugging)
        install(
            FILES "${CMAKE_CURRENT_BINARY_DIR}/bin/${BASE_NAME}.wasm.map"
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            OPTIONAL)
    endif()
endif()
