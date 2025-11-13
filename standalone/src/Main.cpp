
#include "DotNameLib/DotNameLib.hpp"
#include "Assets/AssetContext.hpp"
#include "Utils/Utils.hpp"
#include <cxxopts.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "Logger/ConsoleLogger.hpp"
std::shared_ptr<dotnamecpp::logging::ILogger> logger = std::make_shared<ConsoleLogger> ();
std::unique_ptr<dotnamecpp::v1::DotNameLib> uniqueLib;

#if defined(__EMSCRIPTEN__)
  #include <emscripten/emscripten.h>
#else
  #if defined(DEBUG)
    #include <cpptrace/basic.hpp>
  #endif
#endif

constexpr int helpOutputWidth = 80;

namespace dotnamecpp::AppContext {
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

int handlesArguments (int argc, const char* argv[]) {
  try {
    cxxopts::Options options (argv[0], dotnamecpp::AppContext::standaloneName);

    // Set help output formatting
    options.set_width (helpOutputWidth).set_tab_expansion (true);

    // Define options
    options.add_options () ("h,help", "Show help");
    options.add_options () ("1,omit", "Omit library loading",
        cxxopts::value<bool> ()->default_value ("false"));
    options.add_options () ("2,log2file", "Log to file",
        cxxopts::value<bool> ()->default_value ("false"));

    // Parse arguments
    const auto result = options.parse (argc, argv);

    if (result.count ("help") != 0U) {
      logger->infoStream () << options.help ({ "", "Group" });
      return 0;
    }

    if (result["log2file"].as<bool> ()) {
      static const std::string logFileName
          = std::string (dotnamecpp::AppContext::standaloneName) + ".log";
      logger->enableFileLogging (logFileName);
      logger->infoStream () << "Logging to file enabled [" << logFileName << "]";
    }

    if (!result["omit"].as<bool> ()) {
      uniqueLib = std::make_unique<dotnamecpp::v1::DotNameLib> (std::make_shared<ConsoleLogger> (),
          dotnamecpp::AppContext::getAssetsPath ());
    } else {
      logger->infoStream () << "Loading library omitted [-1]";
    }

    if (!result.unmatched ().empty ()) {
      for ([[maybe_unused]] const auto& arg : result.unmatched ()) {
        logger->infoStream () << "Unrecognized option: " << arg;
      }
      logger->infoStream () << options.help ();
      return 1;
    }
  } catch (const cxxopts::exceptions::exception& e) {
    logger->infoStream () << "error parsing options: " << e.what ();
    return 1;
  }
  return 0;
}

int main (int argc, const char* argv[]) {
  logger->infoStream () << dotnamecpp::AppContext::standaloneName << " starting ...";

#if defined(EMSCRIPTEN)
  logger->info ("Running in Web environment");
  #ifdef __EMSCRIPTEN_PTHREADS__
  logger->info ("pthreads support: Enabled");
  #endif
#endif

  if (handlesArguments (argc, argv) != 0) {
    return 1;
  }

  logger->infoStream () << dotnamecpp::AppContext::standaloneName << " exiting ...";
  return 0;
}