# MIT License Copyright (c) 2024-2025 Tomáš Mark

# ==============================================================================
# Code Coverage Configuration with gcovr
# ==============================================================================

# Coverage option
option(ENABLE_COVERAGE "Enable code coverage analysis" OFF)

# Coverage setup
if(ENABLE_COVERAGE)
    # Check if we're using a compatible compiler
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Enabling code coverage with gcovr")
        
        # Find gcovr executable
        find_program(GCOVR_PATH gcovr)
        if(NOT GCOVR_PATH)
            message(WARNING "gcovr not found! Install with: pip install gcovr")
        else()
            message(STATUS "Found gcovr: ${GCOVR_PATH}")
        endif()
        
        # Coverage compilation flags
        set(COVERAGE_FLAGS --coverage -O0 -g -fprofile-arcs -ftest-coverage)
        
        # Add coverage flags globally
        add_compile_options(${COVERAGE_FLAGS})
        add_link_options(--coverage)
        
        # Set coverage-specific variables
        set(CMAKE_CXX_FLAGS_COVERAGE "-O0 -g --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_C_FLAGS_COVERAGE "-O0 -g --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "--coverage")
        set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "--coverage")
        
        # Mark Coverage as a valid build type
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo" "Coverage")
        
        # Add custom targets for coverage reports (only once)
        if(GCOVR_PATH AND NOT TARGET coverage-html)
            # HTML coverage report
            add_custom_target(coverage-html
                COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} 
                    --html --html-details 
                    -o ${CMAKE_BINARY_DIR}/coverage.html
                    --exclude '${CMAKE_BINARY_DIR}/.*'
                    --exclude '.*/tests/.*'
                    --exclude '.*/test/.*'
                    --exclude '.*/external/.*'
                    --exclude '.*/third_party/.*'
                    --exclude '.*/build/.*'
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating HTML coverage report with gcovr"
                VERBATIM
            )
        endif()
        
        if(GCOVR_PATH AND NOT TARGET coverage-xml)
            # XML coverage report (for CI/CD)
            add_custom_target(coverage-xml
                COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} 
                    --xml 
                    -o ${CMAKE_BINARY_DIR}/coverage.xml
                    --exclude '${CMAKE_BINARY_DIR}/.*'
                    --exclude '.*/tests/.*'
                    --exclude '.*/test/.*'
                    --exclude '.*/external/.*'
                    --exclude '.*/third_party/.*'
                    --exclude '.*/build/.*'
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating XML coverage report with gcovr"
                VERBATIM
            )
        endif()
        
        if(GCOVR_PATH AND NOT TARGET coverage)
            # Combined coverage report (HTML + XML)
            add_custom_target(coverage
                COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} 
                    --html --html-details 
                    -o ${CMAKE_BINARY_DIR}/coverage.html
                    --exclude '${CMAKE_BINARY_DIR}/.*'
                    --exclude '.*/tests/.*'
                    --exclude '.*/test/.*'
                    --exclude '.*/external/.*'
                    --exclude '.*/third_party/.*'
                    --exclude '.*/build/.*'
                COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} 
                    --xml 
                    -o ${CMAKE_BINARY_DIR}/coverage.xml
                    --exclude '${CMAKE_BINARY_DIR}/.*'
                    --exclude '.*/tests/.*'
                    --exclude '.*/test/.*'
                    --exclude '.*/external/.*'
                    --exclude '.*/third_party/.*'
                    --exclude '.*/build/.*'
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating coverage reports (HTML + XML) with gcovr"
                VERBATIM
            )
        endif()
        
        if(GCOVR_PATH AND NOT TARGET coverage-summary)
            # Coverage summary to console
            add_custom_target(coverage-summary
                COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR}
                    --exclude '${CMAKE_BINARY_DIR}/.*'
                    --exclude '.*/tests/.*'
                    --exclude '.*/test/.*'
                    --exclude '.*/external/.*'
                    --exclude '.*/third_party/.*'
                    --exclude '.*/build/.*'
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Displaying coverage summary"
                VERBATIM
            )
        endif()
        
        if(GCOVR_PATH AND NOT TARGET coverage-reset)
            # Zero coverage counters
            add_custom_target(coverage-reset
                COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} --delete
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Resetting coverage counters"
                VERBATIM
            )
        endif()
        
    else()
        message(WARNING "Code coverage is only supported with GCC or Clang compilers")
        set(ENABLE_COVERAGE OFF CACHE BOOL "Enable code coverage analysis" FORCE)
    endif()
endif()

# ==============================================================================
# Helper function to apply coverage settings to targets
# ==============================================================================
function(apply_coverage_settings TARGET_NAME)
    if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TARGET_NAME} PRIVATE ${COVERAGE_FLAGS})
        target_link_options(${TARGET_NAME} PRIVATE --coverage)
        
        # Make sure debug info is preserved
        target_compile_options(${TARGET_NAME} PRIVATE -g)
    endif()
endfunction()
