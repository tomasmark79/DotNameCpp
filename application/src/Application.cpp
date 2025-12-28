#include <DotNameLib/DotNameLib.hpp>
#include <Utils/UtilsFactory.hpp>
#include <cxxopts.hpp>
#include <iostream>

const std::string appName = "DotNameApplication";

int main(int argc, char **argv) {

  using namespace dotnamecpp;
  using namespace dotnamecpp::logging;
  using namespace dotnamecpp::utils;

  constexpr int runFakeDurationSeconds = 1;

  try {
    // cxxopts
    cxxopts::Options options(appName, "DotName C++ Application");
    options.add_options()("h,help", "Print usage");
    options.add_options()("w,write2file", "Write output to file",
                          cxxopts::value<bool>()->default_value("false"));

    auto result = options.parse(argc, argv);
    if (result.count("help") > 0) {
      std::cout << options.help() << '\n';
      return EXIT_SUCCESS;
    }

    auto utilsComponents = UtilsFactory::createAppComponents(
        appName, LoggerConfig{.level = Level::LOG_INFO,
                              .enableFileLogging = result["write2file"].as<bool>(),
                              .logFilePath = "application.log",
                              .colorOutput = true,
                              .appPrefix = appName});

    auto &logger = utilsComponents.logger;
    auto &assetManager = utilsComponents.assetManager;
    auto &platformInfo = utilsComponents.platformInfo;
    auto &customStringsLoader = utilsComponents.customStringsLoader;

    logger->infoStream()
        << "Author: "
        << customStringsLoader->getLocalizedString("Author", "en").value_or("Not found");
    logger->infoStream()
        << "GitHub"
        << ": " << customStringsLoader->getLocalizedString("GitHub", "en").value_or("Not found")
        << " - " << customStringsLoader->getCustomKey("GitHub", "url").value_or("Not found");
    logger->infoStream() << platformInfo->getPlatformName() << " platform detected.";

    // req C++20+
    logger->infoWithLocation(appName + " started ...");

    // Initialize library injecting components like logger and asset manager
    // TODO: Injecting Factory of Factories
    auto library = std::make_unique<v1::DotNameLib>(logger, assetManager);
    if (!library->isInitialized()) {
      logger->errorStream() << "Library initialization failed";
      return EXIT_FAILURE;
    }

    if (!library->run(runFakeDurationSeconds)) {
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
