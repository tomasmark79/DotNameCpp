# SPDX-License-Identifier: MIT
#
# SPDX-FileCopyrightText: Copyright (c) 2019-2023 Lars Melchior and contributors

set(CPM_DOWNLOAD_VERSION 0.42.0)
set(CPM_HASH_SUM "1066e1af6eef1d607bf159ef36a39b4901e2a7dde78529e7a7e34934d03dc118")

if(CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

# Expand relative path. This is important if the provided path contains a tilde (~)
get_filename_component(CPM_DOWNLOAD_LOCATION ${CPM_DOWNLOAD_LOCATION} ABSOLUTE)

# Allow overriding the CPM download URL via environment variable or CMake variable
if(NOT DEFINED CPM_DOWNLOAD_URL)
    if(DEFINED ENV{CPM_DOWNLOAD_URL})
        set(CPM_DOWNLOAD_URL "$ENV{CPM_DOWNLOAD_URL}")
    else()
        set(CPM_DOWNLOAD_URL "https://raw.githubusercontent.com/tomasmark79/CPM.cmake/v${CPM_DOWNLOAD_VERSION}/cmake/CPM.cmake")
    endif()
endif()

file(DOWNLOAD
     ${CPM_DOWNLOAD_URL}
     ${CPM_DOWNLOAD_LOCATION} EXPECTED_HASH SHA256=${CPM_HASH_SUM})

include(${CPM_DOWNLOAD_LOCATION})
