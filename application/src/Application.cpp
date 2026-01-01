#include <DotNameLib/DotNameLib.hpp>
#include <Utils/UtilsFactory.hpp>
#include <cxxopts.hpp>
#include <iostream>

const std::string appName = "DotNameApplication";
const std::string NA = "[Not Found]";

int main(int argc, char **argv) {

  using namespace dotnamecpp;
  using namespace dotnamecpp::logging;
  using namespace dotnamecpp::utils;

  try {
    // ---
    cxxopts::Options options(appName, "DotName C++ Application");
    options.add_options()("h,help", "Print usage");
    options.add_options()("w,write2file", "Write output to file",
                          cxxopts::value<bool>()->default_value("false"));
    auto result = options.parse(argc, argv);
    if (result.count("help") > 0) {
      std::cout << options.help() << '\n';
      return EXIT_SUCCESS;
    }

    // ---
    auto ctx = UtilsFactory::createFullContext(
        appName, LoggerConfig{.level = Level::LOG_INFO,
                              .enableFileLogging = result["write2file"].as<bool>(),
                              .logFilePath = "application.log",
                              .colorOutput = true,
                              .appPrefix = appName});
    // ---
    ctx.logger->infoStream()
        << appName << " (c) "
        << ctx.customStringsLoader->getLocalizedString("Author", "cs").value_or(NA) << " -"
        << ctx.customStringsLoader->getLocalizedString("GitHub", "cs").value_or(NA) << " - "
        << ctx.customStringsLoader->getCustomKey("GitHub", "url").value_or(NA);
    ctx.logger->infoStream() << ctx.platformInfo->getPlatformName() << " platform detected.";

    // ---
    auto library = std::make_unique<dotnamecpp::v1::DotNameLib>(ctx);
    if (!library->isInitialized()) {
      ctx.logger->errorStream() << "Library initialization failed";
      return EXIT_FAILURE;
    }

    // ---
    ctx.logger->infoStream() << appName << " ... shutting down";
    return EXIT_SUCCESS;

  } catch (const std::exception &e) {
    std::cerr << "Fatal error: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
}
