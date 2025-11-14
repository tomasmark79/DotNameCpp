#include <DotNameLib/DotNameLib.hpp>
#include <Utils/UtilsFactory.hpp>
#include <cxxopts.hpp>
#include <iostream>

// Standalone application using DotNameLib
// All components initialized via UtilsFactory

int main (int argc, char** argv) {
  using namespace dotnamecpp;
  using namespace dotnamecpp::logging;
  using namespace dotnamecpp::utils;

  try {
    // Parse command-line options
    cxxopts::Options options ("DotNameStandalone", "DotName C++ Standalone Application");
    options.add_options () ("h,help", "Print usage");
    options.add_options () ("w,write2file", "Write output to file",
                            cxxopts::value<bool> ()->default_value ("false"));

    auto result = options.parse (argc, argv);
    if (result.count ("help") > 0) {
      std::cout << options.help () << '\n';
      return EXIT_SUCCESS;
    }

    // Get executable path
    auto platformInfo = UtilsFactory::createPlatformInfo ();
    auto execPathResult = platformInfo->getExecutablePath ();
    if (!execPathResult.hasValue ()) {
      std::cerr << "Failed to get executable path: " << execPathResult.error ().toString () << '\n';
      return EXIT_FAILURE;
    }

    // Initialize logger
    LoggerConfig loggerConfig{ .level = Level::LOG_INFO,
                               .enableFileLogging = result["write2file"].as<bool> (),
                               .logFilePath = "standalone.log",
                               .colorOutput = true };
    auto logger = UtilsFactory::createLogger (LoggerType::Console, loggerConfig);

    // Initialize assets
    auto assetManager =
      UtilsFactory::createAssetManager (execPathResult.value (), "DotNameStandalone");

    if (!assetManager->validate ()) {
      logger->errorStream () << "Failed to validate assets: " << assetManager->getAssetsPath ();
      return EXIT_FAILURE;
    }

    logger->infoStream () << "Assets initialized: " << assetManager->getAssetsPath ();

    // Initialize library
    auto library = std::make_unique<v1::DotNameLib> (logger, assetManager);
    logger->infoStream () << "Library initialized successfully";

    // Run application logic
    logger->warningStream () << "... warning example ...";
    logger->errorStream () << "... error example ...";
    logger->infoStream () << "DotNameStandalone running...";
    logger->infoStream () << ".";

    logger->infoStream () << "DotNameStandalone shutting down";
    return EXIT_SUCCESS;

  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what () << '\n';
    return EXIT_FAILURE;
  }
}
