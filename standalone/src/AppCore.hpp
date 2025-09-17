// MIT License Copyright (c) 2024-2025 Tomáš Mark

#include "DotNameLib/DotNameLib.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Utils.hpp"
#include "Assets/AssetContext.hpp"

#include <cxxopts.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#if defined(__EMSCRIPTEN__)
  #include <emscripten/emscripten.h>
#else
  #include <cpptrace/cpptrace.hpp>
void trace () {
  cpptrace::generate_trace ().print ();
}
#endif

using namespace DotNameUtils;

namespace AppContext {
  constexpr char standaloneName[] = "DotNameStandalone";
  const std::filesystem::path standalonePath = PathUtils::getStandalonePath ();
  const std::filesystem::path assetsPath
      = AssetContext::findAssetsPath (standalonePath, standaloneName);
  constexpr std::string_view utilsFirstAssetFile = UTILS_FIRST_ASSET_FILE;
  const std::filesystem::path assetsPathFirstFile = assetsPath / utilsFirstAssetFile;
}

std::unique_ptr<dotname::DotNameLib> uniqueLib;

int handlesArguments (int argc, const char* argv[]) {
  try {
    auto options = std::make_unique<cxxopts::Options> (argv[0], AppContext::standaloneName);
    options->positional_help ("[optional args]").show_positional_help ();
    options->set_width (80);
    options->set_tab_expansion ();
    options->add_options () ("h,help", "Show help");
    options->add_options () ("1,omit", "Omit library loading",
                             cxxopts::value<bool> ()->default_value ("false"));
    options->add_options () ("2,log2file", "Log to file",
                             cxxopts::value<bool> ()->default_value ("false"));
    options->add_options () ("3,cpubench", "Run single&multi core cpubench",
                             cxxopts::value<bool> ()->default_value ("false"));

    const auto result = options->parse (argc, argv);

    if (result.count ("help")) {
      LOG_I_STREAM << options->help ({ "", "Group" }) << std::endl;
      return 0;
    }

    if (result["log2file"].as<bool> ()) {
      LOG.enableFileLogging (std::string (AppContext::standaloneName) + ".log");
      LOG_D_STREAM << "Logging to file enabled [-2]" << std::endl;
    }

    if (!result.count ("omit")) {
      // uniqueLib = std::make_unique<dotname::DotNameLib> ();
      uniqueLib = std::make_unique<dotname::DotNameLib> (AppContext::assetsPath);
    } else {
      LOG_D_STREAM << "Loading library omitted [-1]" << std::endl;
    }

    if (result["cpubench"].as<bool> ()) {
      Performance::parUnseqHeavyCalculation (
          3.14159); // uses parallel execution on Linux (-ltbb), fallback on other platforms
    }

    if (!result.unmatched ().empty ()) {
      for (const auto& arg : result.unmatched ()) {
        LOG_E_STREAM << "Unrecognized option: " << arg << std::endl;
      }
      LOG_I_STREAM << options->help () << std::endl;
      return 1;
    }

  } catch (const cxxopts::exceptions::exception& e) {
    LOG_E_STREAM << "error parsing options: " << e.what () << std::endl;
    return 1;
  }
  return 0;
}

// unused right now
int printAssets (const std::filesystem::path& assetsPath) {
  try {
    auto files = FileManager::listFiles (assetsPath);
    if (files.empty ()) {
      LOG_D_STREAM << "No assets found in " << assetsPath << std::endl;
      return 0;
    }

    for (const auto& file : files) {
      LOG_D_STREAM << file << std::endl;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what () << std::endl;
    return 1;
  }
  return 0;
}

int runApp (int argc, const char* argv[]) {

  LOG.noHeader (true);
  LOG.setAddNewLine (false);
  LOG_I_STREAM << "Starting " << AppContext::standaloneName << " ..." << std::endl;

#ifdef EMSCRIPTEN
  LOG_I_STREAM << "C++ Running in Emscripten environment" << std::endl;
#endif

#ifdef __EMSCRIPTEN_PTHREADS__
  LOG_I_STREAM << "Emscripten C++ with pthreads support" << std::endl;
#endif

  if (handlesArguments (argc, argv) != 0) {
    return 1;
  }

  // I know it is smartpointer, but ... why not
  uniqueLib = nullptr;

  // demo error
  LOG_E_STREAM << "This is a demo error message" << std::endl;

#ifndef EMSCRIPTEN
  // demo trace
  trace ();
#endif

  // bye
  LOG_I_STREAM << "Sucessfully exited " << AppContext::standaloneName << std::endl;
  return 0;
}