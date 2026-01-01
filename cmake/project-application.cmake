# MIT License Copyright (c) 2024-2026 Tomáš Mark

# ==============================================================================
# APPLICATION-SPECIFIC CONFIGURATION
# ==============================================================================
include(${CMAKE_CURRENT_LIST_DIR}/project-common.cmake)

project(
    ${APPLICATION_NAME}
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
# Application dependencies
# ==============================================================================
# Note: DotNameLib target should be available from orchestrator
CPMAddPackage("gh:tomasmark79/CPMLicenses.cmake@0.0.7")
cpm_licenses_create_disclaimer_target(
    write-licenses-${APPLICATION_NAME}
    "${CMAKE_CURRENT_BINARY_DIR}/${APPLICATION_NAME}_third_party.txt" "${CPM_PACKAGES}")
CPMAddPackage(
    GITHUB_REPOSITORY jarro2783/cxxopts
    VERSION 3.2.1
    OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL NO")

# cpptrace removed due to C++20 modules incompatibility with Unix Makefiles generator

# ==============================================================================
# Application source files
# ==============================================================================
gather_sources(sources ${CMAKE_CURRENT_SOURCE_DIR}/application/src)
list(APPEND sources ${CMAKE_CURRENT_SOURCE_DIR}/application/src/Application.cpp)

# ==============================================================================
# Create application target
# ==============================================================================
add_executable(${APPLICATION_NAME})
target_sources(${APPLICATION_NAME} PRIVATE ${sources})

# Apply common target settings
apply_common_target_settings(${APPLICATION_NAME})

# Link with library
target_link_libraries(${APPLICATION_NAME} PRIVATE DotNameLib cxxopts::cxxopts)

# ==============================================================================
# Asset processing and Emscripten configuration
# ==============================================================================
include(${CMAKE_CURRENT_LIST_DIR}/tmplt-assets.cmake)
apply_assets_processing_application()

include(${CMAKE_CURRENT_LIST_DIR}/tmplt-emscripten.cmake)
emscripten(${APPLICATION_NAME} 1 1 "")

# ==============================================================================
# Tests configuration
# ==============================================================================
if(ENABLE_GTESTS)
    message(STATUS "GTESTS enabled")
    include(CTest) # Enable testing at the root level
    add_library(${TEST_NAME_LOWER}_application_common INTERFACE)
    target_link_libraries(${TEST_NAME_LOWER}_application_common INTERFACE ${LIBRARY_NAME}
                                                                          cxxopts::cxxopts)

    add_library(dotname::${TEST_NAME_LOWER}_application_common ALIAS
                ${TEST_NAME_LOWER}_application_common)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/application/tests tests)
endif()

# ==============================================================================
# Installation
# ==============================================================================
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR})

# Standard installation for native platforms
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    install(TARGETS ${APPLICATION_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
    # Special installation for Emscripten - install all generated files
    install(TARGETS ${APPLICATION_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    # Install additional Emscripten files (js, wasm, data) These files are generated alongside the
    # .html file
    get_target_property(TARGET_SUFFIX ${APPLICATION_NAME} SUFFIX)
    if(TARGET_SUFFIX STREQUAL ".html")
        set(BASE_NAME "${APPLICATION_NAME}")

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
