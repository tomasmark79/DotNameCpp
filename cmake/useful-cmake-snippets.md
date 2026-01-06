# A DotName collection of useful and tested CMake snippets  

## [fmt - Formatting library for C++](https://github.com/fmtlib/fmt)
```cmake
# find_package(fmt QUIET) # Disabled to always use CPM to avoid conflicts
if(NOT fmt_FOUND)
    CPMAddPackage(
`        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 12.1.0
        OPTIONS "FMT_INSTALL YES" "FMT_TEST NO" "FMT_DOC NO")
endif()
target_link_libraries( ${LIBRARY_NAME} PUBLIC fmt::fmt)
```

## [nlohmann/json - JSON for Modern C++](https://github.com/nlohmann/json)
```cmake    
# find_package(nlohmann_json QUIET) # Disabled to always use CPM to avoid conflicts
if(NOT TARGET nlohmann_json AND NOT TARGET nlohmann_json::nlohmann_json)
    CPMAddPackage("gh:nlohmann/json@3.12.0")
    if(TARGET nlohmann_json)
        install(TARGETS nlohmann_json EXPORT ${LIBRARY_NAME}Targets)
    endif()
endif()
target_link_libraries( ${LIBRARY_NAME} PUBLIC nlohmann_json::nlohmann_json)
```

## [juce-framework/JUCE](https://github.com/juce-framework/JUCE)
```cmake
CPMAddPackage(
    GITHUB_REPOSITORY juce-framework/JUCE
    GIT_TAG 8.0.12
    OPTIONS 
        "JUCE_MODULES_ONLY OFF" 
        "JUCE_BUILD_EXTRAS ON" 
        "JUCE_BUILD_EXAMPLES ON")
# Exclude WebViewPluginDemo_LV2 from default build (requires X11 display in subprocess)
if(TARGET WebViewPluginDemo_LV2)
    set_target_properties(WebViewPluginDemo_LV2 PROPERTIES EXCLUDE_FROM_ALL TRUE)
    message(STATUS "Excluding WebViewPluginDemo_LV2 from default build (requires X11 display)")
endif()
target_link_libraries( ${LIBRARY_NAME} PRIVATE $<BUILD_INTERFACE:juce::juce_recommended_warning_flags>)
```

## [EmojiModule - C++ library for handling emojis](https://github.com/tomasmark79/EmojiModule)
```cmake
CPMAddPackage(
    NAME EmojiModule
    GITHUB_REPOSITORY tomasmark79/EmojiModule
    GIT_TAG main
    # file(COPY ${EmojiModule_SOURCE_DIR}/assets/emoji-test.txt DESTINATION
    # ${CMAKE_CURRENT_SOURCE_DIR}/assets)
)
target_link_libraries( ${LIBRARY_NAME} PUBLIC EmojiModuleLib )
```

## [Dpp - C++ Discord API Bot Library](https://github.com/brainboxdotcc/DPP)
```cmake
# https://github.com/brainboxdotcc/DPP/blob/master/CMakeLists.txt important "CONAN_EXPORTED ON" has
# to be used together with Conan
CPMAddPackage(
    NAME dpp
    GITHUB_REPOSITORY brainboxdotcc/DPP
    GIT_TAG v10.1.4
    OPTIONS "BUILD_SHARED_LIBS ON" "DPP_BUILD_TEST OFF" "DPP_INSTALL OFF" "DPP_NO_CORO OFF"
            "CONAN_EXPORTED ON")
# equivalent to: "DPP_INSTALL ON" in CPM for DPP Target - kept for example
if(TARGET dpp)
    install(TARGETS dpp EXPORT ${LIBRARY_NAME}Targets)
    target_compile_options(dpp INTERFACE -Wno-unused-parameter -Wno-unused-variable)
endif()
target_link_libraries( ${LIBRARY_NAME} PUBLIC dpp)
```