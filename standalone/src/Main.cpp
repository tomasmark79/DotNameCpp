#include "Standalone.hpp"
#include <Utils/UtilsFactory.hpp>
#include <cxxopts.hpp>
#include <iostream>

// Flow
// Main.cpp ->
// Standalone.hpp -> [Standalone.cpp (All implementations are inline)]
// DotNameLib.hpp -> [DotNameLib.cpp (All implementations are inline)]

// Namespace structure:
// dotnamecpp::
//   ├── v1::              // Main library
//   ├── app::             // Standalone application
//   ├── logging::         // Logging subsystem
//   ├── assets::          // Asset management
//   └── utils::           // Utils subsystem
//       ├── IFileReader, IFileWriter, IPathResolver, IDirectoryManager
//       ├── IPlatformInfo (Windows, Unix, Emscripten)
//       ├── IJsonSerializer, ICustomStringsLoader
//       └── IStringFormatter

std::filesystem::path getStandalonePath () {
  auto platformInfo = dotnamecpp::utils::UtilsFactory::createPlatformInfo ();
  auto result = platformInfo->getExecutablePath ();
  if (!result.hasValue ()) {
    throw std::runtime_error ("Failed to get executable path: " + result.error ().toString ());
  }
  return result.value ();
}

int main (int argc, char** argv) {
  using namespace dotnamecpp::app;
  using namespace dotnamecpp::logging;

  try {

    Standalone app;

    // Parse command-line options
    cxxopts::Options options (app.getAppName (), "DotName C++ Standalone Application");
    options.add_options () ("h,help", "Print usage");
    options.add_options () ("w,write2file", "Write output to file",
                            cxxopts::value<bool> ()->default_value ("false"));
    auto result = options.parse (argc, argv);
    if (result.count ("help") > 0) {
      std::cout << options.help () << '\n';
      return EXIT_SUCCESS;
    }

    LoggerConfig loggerConfig{ .level = Level::LOG_INFO,
                               .enableFileLogging = result["write2file"].as<bool> (),
                               .logFilePath = "standalone.log",
                               .colorOutput = true };

    if (!app.initializeComponents (loggerConfig, getStandalonePath ())) {
      std::cerr << "Failed to initialize application\n";
      return EXIT_FAILURE;
    }

    return app.run ();

  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what () << '\n';
    return EXIT_FAILURE;
  }

  //
}