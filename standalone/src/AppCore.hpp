#pragma once

#include "DotNameLib/DotNameLib.hpp"
#include "Assets/AssetContext.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Utils.hpp"
#include <cxxopts.hpp>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#if defined(__EMSCRIPTEN__)
  #include <emscripten/emscripten.h>
#else
  #if defined(DEBUG)
    #include <cpptrace/basic.hpp>
  #endif
#endif

constexpr int maxArguments = 128;
constexpr int helpOutputWidth = 80;

namespace AppContext {
  constexpr const char* standaloneName = "DotNameStandalone";
  constexpr std::string_view utilsFirstAssetFile = UTILS_FIRST_ASSET_FILE;
  inline const std::filesystem::path& getStandalonePath () {
    static const std::filesystem::path standalonePath
        = DotNameUtils::fs::path::getStandalonePath ();
    return standalonePath;
  }
  inline const std::filesystem::path& getAssetsPath () {
    static const std::filesystem::path assetsPath
        = AssetContext::findAssetsPath (getStandalonePath (), standaloneName);
    return assetsPath;
  }
  inline const std::filesystem::path& getAssetsPathFirstFile () {
    static const std::filesystem::path assetsPathFirstFile = getAssetsPath () / utilsFirstAssetFile;
    return assetsPathFirstFile;
  }
}

std::unique_ptr<dotname::DotNameLib> uniqueLib;

inline int handlesArguments (int argc, const char* argv[]) {
  try {
    cxxopts::Options options (argv[0], AppContext::standaloneName);
    options.set_width (helpOutputWidth);
    options.set_tab_expansion ();
    options.add_options () ("h,help", "Show help");
    options.add_options () ("1,omit", "Omit library loading",
        cxxopts::value<bool> ()->default_value ("false"));
    options.add_options () ("2,log2file", "Log to file",
        cxxopts::value<bool> ()->default_value ("false"));

    const auto result = options.parse (argc, argv);

    if (result.count ("help") != 0U) {
      LOG_I_STREAM << options.help ({ "", "Group" }) << "\n";
      return 0;
    }

    if (result["log2file"].as<bool> ()) {
      static const std::string logFileName = std::string (AppContext::standaloneName) + ".log";
      LOG.enableFileLogging (logFileName);
      LOG_I_STREAM << "Logging to file enabled [-2]" << "\n";
    }

    if (!result["omit"].as<bool> ()) {
      uniqueLib = std::make_unique<dotname::DotNameLib> (AppContext::getAssetsPath ());
    } else {
      LOG_I_STREAM << "Loading library omitted [-1]" << "\n";
    }

    if (!result.unmatched ().empty ()) {
      for (const auto& arg : result.unmatched ()) {
        LOG_E_STREAM << "Unrecognized option: " << arg << "\n";
      }
      LOG_I_STREAM << options.help () << "\n";
      return 1;
    }
  } catch (const cxxopts::exceptions::exception& e) {
    LOG_E_STREAM << "error parsing options: " << e.what () << "\n";
    return 1;
  }
  return 0;
}

// unused right now
inline int printAssets (const std::filesystem::path& assetsPath) {
  try {
    auto files = DotNameUtils::fs::mgmt::listFiles (assetsPath);
    if (files.empty ()) {
      LOG_D_STREAM << "No assets found in " << assetsPath << "\n";
      return 0;
    }

    for (const auto& file : files) {
      LOG_D_STREAM << file << "\n";
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what () << "\n";
    return 1;
  }
  return 0;
}

inline int runApp (int argc, const char* argv[]) {

  LOG.noHeader (true);
  LOG.setAddNewLine (false);
  LOG_I_STREAM << "Starting " << AppContext::standaloneName << " ..." << "\n";

#ifdef EMSCRIPTEN
  LOG_I_STREAM << "Running in Web environment" << "\n";
  #ifdef __EMSCRIPTEN_PTHREADS__
  LOG_I_STREAM << "pthreads support: Enabled" << "\n";
  #endif
#else
  #if defined(DEBUG)
  LOG_D_STREAM << cpptrace::generate_trace ();
  #endif
#endif

  if (handlesArguments (argc, argv) != 0) {
    return 1;
  }

  // bye
  LOG_I_STREAM << "Successfully exited " << AppContext::standaloneName << "\n";
  return 0;
}

// MIT License Copyright (c) 2024-2025 Tomáš Mark
