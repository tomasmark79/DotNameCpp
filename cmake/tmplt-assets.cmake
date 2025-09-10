# MIT License Copyright (c) 2024-2025 Tomáš Mark

# This CMake script will copy and install assets for the standalone application. The assets are
# copied from the source directory to the build directory and installed to the installation
# directory. The asset paths are defined as compilation definitions for the standalone application.

function(copy_assets target asset_sources destination)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${destination}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${asset_sources} "${destination}")
endfunction()

function(apply_assets_processing_standalone)

    # Source destination
    set(ASSET_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets/")

    # Configuration dependent asset paths by platform
    if(WIN32)
        set(ASSET_INSTALL_DIR "assets")
        set(ASSET_BUILD_DIR "$<TARGET_FILE_DIR:${STANDALONE_NAME}>/${ASSET_INSTALL_DIR}")
        set(INSTALL_DESTINATION "bin/${ASSET_INSTALL_DIR}")
        set(ASSET_PATH_DEFINE "${ASSET_INSTALL_DIR}")
    else() # UNIX/APPLE
        set(ASSET_INSTALL_DIR "share/${STANDALONE_NAME}/assets")
        set(ASSET_BUILD_DIR "$<TARGET_FILE_DIR:${STANDALONE_NAME}>/../${ASSET_INSTALL_DIR}")
        set(INSTALL_DESTINATION "${ASSET_INSTALL_DIR}")
        set(ASSET_PATH_DEFINE "assets") # Will be resolved dynamically at runtime
    endif()

    # Check if assets exist
    file(GLOB_RECURSE ASSET_FILES "${ASSET_SOURCE_DIR}/*")

    if(NOT ASSET_FILES)
        message(STATUS "No asset files found in ${ASSET_SOURCE_DIR}.")
        target_compile_definitions(
            ${STANDALONE_NAME} PRIVATE ASSET_FILES="No asset files found in ${ASSET_SOURCE_DIR}.")
        return()
    endif()

    # Extract asset file names
    set(ASSET_FILE_NAMES "")

    set(increment 0)
    foreach(ASSET_FILE ${ASSET_FILES})
        get_filename_component(FILE_NAME ${ASSET_FILE} NAME)
        list(APPEND ASSET_FILE_NAMES ${FILE_NAME})
        if(increment EQUAL 0)
            set(FIRST_ASSET_FILE ${FILE_NAME})
            math(EXPR increment "${increment} + 1")
        endif()
    endforeach()

    string(REPLACE ";" "," ASSET_FILE_NAMES_STR "${ASSET_FILE_NAMES}")

    # Copy and install assets
    # Copy assets to build directory for development (both Windows and Unix/Linux)
    copy_assets(${STANDALONE_NAME} "${ASSET_SOURCE_DIR}" "${ASSET_BUILD_DIR}")
    
    install(DIRECTORY "${ASSET_SOURCE_DIR}" DESTINATION "${INSTALL_DESTINATION}" 
            FILES_MATCHING PATTERN "*")

    # Set compilation definitions for asset paths
    target_compile_definitions(
        ${STANDALONE_NAME}
        PRIVATE UTILS_ASSET_PATH="${ASSET_PATH_DEFINE}"
                UTILS_FIRST_ASSET_FILE="${FIRST_ASSET_FILE}"
                UTILS_ASSET_FILES="${ASSET_FILE_NAMES_STR}")
endfunction()
