#include <DotNameLib/DotNameLib.hpp>
#include <Utils/UtilsFactory.hpp>
#include <cxxopts.hpp>
#include <iostream>

// Application application using DotNameLib
// All components initialized via UtilsFactory

int main(int argc, char **argv) {
  // appname
  const std::string appName = "DotNameApplication";
  using namespace dotnamecpp;
  using namespace dotnamecpp::logging;
  using namespace dotnamecpp::utils;

  constexpr int runDurationSeconds = 5;

  try {
    // Parse command-line options
    cxxopts::Options options(appName, "DotName C++ Application");
    options.add_options()("h,help", "Print usage");
    options.add_options()("w,write2file", "Write output to file",
                          cxxopts::value<bool>()->default_value("false"));

    auto result = options.parse(argc, argv);
    if (result.count("help") > 0) {
      std::cout << options.help() << '\n';
      return EXIT_SUCCESS;
    }

    // Get executable path
    auto platformInfo = UtilsFactory::createPlatformInfo();
    auto execPathResult = platformInfo->getExecutablePath();
    if (!execPathResult.hasValue()) {
      std::cerr << "Failed to get executable path: " << execPathResult.error().toString() << '\n';
      return EXIT_FAILURE;
    }

    // Initialize logger
    LoggerConfig loggerConfig{.level = Level::LOG_INFO,
                              .enableFileLogging = result["write2file"].as<bool>(),
                              .logFilePath = "application.log",
                              .colorOutput = true,
                              .appPrefix = appName};
    auto logger = UtilsFactory::createLogger(LoggerType::Console, loggerConfig);

#if __cplusplus >= 202002L
    // Available in C++20 and later
    logger->infoWithLocation(appName + " started ...");
#else
    logger->infoStream() << appName << " started ...";
#endif

    auto assetManager = UtilsFactory::createAssetManager(execPathResult.value(), appName);
    if (!assetManager->validate()) {
      logger->errorStream() << "Failed to validate assets: " << assetManager->getAssetsPath();
      return EXIT_FAILURE;
    }

    // Initialize library
    auto library = std::make_unique<v1::DotNameLib>(logger, assetManager);
    if (!library->isInitialized()) {
      logger->errorStream() << "Library initialization failed";
      return EXIT_FAILURE;
    }

    if (!library->run(runDurationSeconds)) {
      logger->errorStream() << "Failed to run library";
      return EXIT_FAILURE;
    }

    logger->infoStream() << appName << " ... shutting down";
    return EXIT_SUCCESS;

  } catch (const std::exception &e) {
    std::cerr << "Fatal error: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
}
